#include "jit_interface.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "codegen.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/Error.h>
#include <llvm/IR/LegacyPassManager.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

RootProgram* rootProgram = nullptr;
extern int yyparse();
extern FILE* yyin;

static bool parseSource(const std::string& source_code) {
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

    llvm::FunctionType* mainType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);
    llvm::Function* mainFunc = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*context, "entry", mainFunc);
    builder.SetInsertPoint(BB);
    CodegenContext cgCtx(*context, builder, *module);
    for (const auto& stmt : rootProgram->statements) {
        if (!stmt) continue;
        stmt->codegen(cgCtx);
    }
    builder.CreateRet(llvm::ConstantInt::get(*context, llvm::APInt(32, 0)));

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

    llvm::FunctionType* mainType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);
    llvm::Function* mainFunc = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*context, "entry", mainFunc);
    builder.SetInsertPoint(BB);
    CodegenContext cgCtx(*context, builder, *module);
    for (const auto& stmt : rootProgram->statements) {
        if (!stmt) continue;
        stmt->codegen(cgCtx);
    }
    builder.CreateRet(llvm::ConstantInt::get(*context, llvm::APInt(32, 0)));

    std::string errStr;
    llvm::EngineBuilder builderEngine(std::move(module));
    builderEngine.setErrorStr(&errStr);
    builderEngine.setEngineKind(llvm::EngineKind::JIT);
    builderEngine.setMCJITMemoryManager(std::make_unique<llvm::SectionMemoryManager>());
    std::unique_ptr<llvm::ExecutionEngine> execEngine(builderEngine.create());

    if (!execEngine) {
        return "Failed to create ExecutionEngine: " + errStr;
    }

    execEngine->finalizeObject();

    // Pipe stdout
    int pipefd[2];
    pipe(pipefd);
    int stdout_copy = dup(STDOUT_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue result = execEngine->runFunction(mainFunc, noargs);

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

std::string compile_and_get_ir(const std::string& source_code) {
    return generateIR(source_code);
}

std::string run_jit_and_get_output(const std::string& source_code) {
    return executeJIT(source_code);
}

