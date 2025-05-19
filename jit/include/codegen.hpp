#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>  // For llvm::Value*
#include <map>
#include <memory>
#include <string>

// Forward declaration of AST node class
class RootProgram;

class CodegenContext {
public:
    llvm::LLVMContext& context;
    llvm::IRBuilder<>& builder;
    llvm::Module& module;

    // 🔧 Symbol table for variables
    std::map<std::string, llvm::AllocaInst*> NamedValues;

    CodegenContext(llvm::LLVMContext& ctx, llvm::IRBuilder<>& b, llvm::Module& m)
        : context(ctx), builder(b), module(m) {}
};

#endif // CODEGEN_HPP

