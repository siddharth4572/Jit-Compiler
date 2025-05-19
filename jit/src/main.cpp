#include "parser.hpp"
#include "ast.hpp"
#include "codegen.hpp"

// LLVM headers
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/Error.h>

#include <iostream>
#include <fstream>
#include <memory>

// From parser.y
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

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source-file>\n";
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        std::cerr << "Could not open file: " << argv[1] << "\n";
        return 1;
    }

    yyin = file;
    if (yyparse() != 0) {
        std::cerr << "Parsing failed.\n";
        fclose(file);
        return 1;
    }
    fclose(file);

    if (!rootProgram) {
        std::cerr << "No AST was generated.\n";
        return 1;
    }

    std::cout << "Parsed successfully.\n";

    // === LLVM setup ===
    auto TSCtx = std::make_unique<llvm::LLVMContext>();
    llvm::LLVMContext& TheContext = *TSCtx;
    auto TheModule = std::make_unique<llvm::Module>("jit", TheContext);
    llvm::IRBuilder<> IRB(TheContext);

    // Declare printf
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(TheContext),
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0),
        true
    );
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", TheModule.get());

    // Wrap rootProgram statements in a synthetic main
    createMainWrapper(rootProgram->statements, TheContext, IRB, *TheModule);

    std::cout << "\n>>> Generated LLVM IR:\n";
    TheModule->print(llvm::outs(), nullptr);

    std::error_code EC;
    llvm::raw_fd_ostream outFile("output.ll", EC, llvm::sys::fs::OF_None);
    TheModule->print(outFile, nullptr);
    outFile.close();

    // === JIT ===
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::orc::LLJITBuilder JITBuilder;

    // Set custom object linking layer to fix EHFrame registration
    JITBuilder.setObjectLinkingLayerCreator(
        [](llvm::orc::ExecutionSession& ES, const llvm::Triple& TT) {
            auto ObjLayer = std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(
                ES, []() { return std::make_unique<llvm::SectionMemoryManager>(); }
            );
            ObjLayer->setAutoClaimResponsibilityForObjectSymbols(true);
            ObjLayer->setProcessAllSections(true);
            return ObjLayer;
        }
    );

    auto JIT = llvm::cantFail(JITBuilder.create());

    llvm::cantFail(JIT->addIRModule(
        llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TSCtx))
    ));

    auto sym = JIT->lookup("main");
    if (!sym) {
        std::cerr << "Function 'main' not found in JIT.\n";
        return 1;
    }

    auto* mainFuncPtr = sym->toPtr<int (*)()>();
    int result = mainFuncPtr();


    return 0;
}

