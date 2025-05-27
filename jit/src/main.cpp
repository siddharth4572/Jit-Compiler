#include "parser.hpp"
#include "ast.hpp"
#include "codegen.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <unistd.h>

RootProgram* rootProgram = nullptr;
extern int yyparse();
extern FILE* yyin;

llvm::Function* createMainWrapper(const std::vector<std::unique_ptr<Stmt>>& stmts, llvm::LLVMContext& ctx, llvm::IRBuilder<>& builder, llvm::Module& module) {
    llvm::FunctionType* mainType = llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), false);
    llvm::Function* mainFunc = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", &module);

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(ctx, "entry", mainFunc);
    builder.SetInsertPoint(BB);

    CodegenContext cgCtx(ctx, builder, module);
    for (const auto& stmt : stmts) {
        if (!stmt) {
            std::cerr << "Null statement in AST.\n";
            continue;
        }
        llvm::Value* result = stmt->codegen(cgCtx);
        if (!result) {
            std::cerr << "Codegen failed for a statement.\n";
        }
    }

    builder.CreateRet(llvm::ConstantInt::get(ctx, llvm::APInt(32, 0)));
    return mainFunc;
}

bool parseSource(const std::string& source_code) {
    FILE* in = fmemopen((void*)source_code.c_str(), source_code.size(), "r");
    if (!in) return false;
    yyin = in;
    rootProgram = nullptr;
    bool success = yyparse() == 0 && rootProgram != nullptr;
    fclose(in);
    return success;
}

std::string generateIR(const std::string& source_code) {
    if (!parseSource(source_code)) {
        return "Parsing failed.\n";
    }

    auto context = std::make_unique<llvm::LLVMContext>();
    llvm::IRBuilder<> builder(*context);
    auto module = std::make_unique<llvm::Module>("jit", *context);

    // Declare printf
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0),
        true
    );
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module.get());

    createMainWrapper(rootProgram->statements, *context, builder, *module);

    std::string irStr;
    llvm::raw_string_ostream oss(irStr);
    module->print(oss, nullptr);
    oss.flush();
    return irStr;
}

std::string executeJIT(const std::string& source_code) {
    if (!parseSource(source_code)) {
        return "Parsing failed.\n";
    }

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    auto context = std::make_unique<llvm::LLVMContext>();
    llvm::IRBuilder<> builder(*context);
    auto module = std::make_unique<llvm::Module>("jit", *context);

    // Declare printf
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0),
        true
    );
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module.get());

    createMainWrapper(rootProgram->statements, *context, builder, *module);

    std::string errStr;
    llvm::EngineBuilder builderEngine(std::move(module));
    builderEngine.setEngineKind(llvm::EngineKind::JIT);
    builderEngine.setErrorStr(&errStr);
    builderEngine.setOptLevel(llvm::CodeGenOpt::Default);

    builderEngine.setMCJITMemoryManager(
        std::unique_ptr<llvm::RTDyldMemoryManager>(new llvm::SectionMemoryManager())
    );

    llvm::ExecutionEngine* execEngine = builderEngine.create();
    if (!execEngine) {
        return "Failed to create ExecutionEngine: " + errStr + "\n";
    }

    execEngine->finalizeObject();

    llvm::Function* mainFn = execEngine->FindFunctionNamed("main");
    if (!mainFn) {
        return "Function 'main' not found.\n";
    }

    // Capture printf() output via pipe
    int pipefd[2];
    pipe(pipefd);
    int stdout_copy = dup(STDOUT_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    std::vector<llvm::GenericValue> args;
    llvm::GenericValue result = execEngine->runFunction(mainFn, args);

    fflush(stdout);
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdout_copy);

    char buffer[4096];
    ssize_t count = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);

    std::string output;
    if (count > 0) {
        buffer[count] = '\0';
        output = buffer;
    }

    return output;
}

int main(int argc, char** argv) {
    std::string inputCode;

    if (argc > 1) {
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Error opening file: " << argv[1] << "\n";
            return 1;
        }
        std::ostringstream oss;
        oss << file.rdbuf();
        inputCode = oss.str();
    } else {
        std::cout << "Enter code (end with Ctrl+D):\n";
        std::ostringstream oss;
        oss << std::cin.rdbuf();
        inputCode = oss.str();
    }

    std::cout << "--- LLVM IR ---\n";
    std::cout << generateIR(inputCode) << "\n";

    std::cout << "--- JIT Execution Output ---\n";
    std::cout << executeJIT(inputCode) << "\n";

    return 0;
}

