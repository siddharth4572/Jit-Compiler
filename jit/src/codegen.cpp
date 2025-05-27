#include "ast.hpp"
#include "codegen.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APFloat.h"
#include <map>
#include <iostream>

using namespace llvm;

// ---------------- Expressions ----------------

Value* Number::codegen(CodegenContext& ctx) {
    return ConstantFP::get(ctx.context, APFloat(value));
}

Value* Variable::codegen(CodegenContext& ctx) {
    auto it = ctx.NamedValues.find(name);
    if (it == ctx.NamedValues.end() || !it->second) {
        std::cerr << "Error: Variable '" << name << "' used before assignment.\n";
        return nullptr;
    }
    return ctx.builder.CreateLoad(it->second->getAllocatedType(), it->second, name.c_str());
}

Value* BinaryExpr::codegen(CodegenContext& ctx) {
    Value* L = lhs->codegen(ctx);
    Value* R = rhs->codegen(ctx);
    if (!L || !R) return nullptr;

    if (op == "+") return ctx.builder.CreateFAdd(L, R, "addtmp");
    if (op == "-") return ctx.builder.CreateFSub(L, R, "subtmp");
    if (op == "*") return ctx.builder.CreateFMul(L, R, "multmp");
    if (op == "/") return ctx.builder.CreateFDiv(L, R, "divtmp");

    Value* cmp = nullptr;
    if (op == "<") cmp = ctx.builder.CreateFCmpULT(L, R, "cmptmp");
    else if (op == ">") cmp = ctx.builder.CreateFCmpUGT(L, R, "cmptmp");
    else if (op == "==") cmp = ctx.builder.CreateFCmpUEQ(L, R, "cmptmp");
    else if (op == "!=") cmp = ctx.builder.CreateFCmpUNE(L, R, "cmptmp");
    else if (op == "and" || op == "or") {
        auto zero = ConstantFP::get(ctx.context, APFloat(0.0));
        Value* lbool = ctx.builder.CreateFCmpONE(L, zero, "lbool");
        Value* rbool = ctx.builder.CreateFCmpONE(R, zero, "rbool");
        cmp = (op == "and")
            ? ctx.builder.CreateAnd(lbool, rbool, "andtmp")
            : ctx.builder.CreateOr(lbool, rbool, "ortmp");
    } else {
        std::cerr << "Error: Unknown binary operator '" << op << "'\n";
        return nullptr;
    }

    return ctx.builder.CreateUIToFP(cmp, Type::getDoubleTy(ctx.context), "booltmp");
}

Value* FunctionCall::codegen(CodegenContext& ctx) {
    if (callee == "print") {
        if (args.size() != 1) {
            std::cerr << "Error: print expects 1 argument\n";
            return nullptr;
        }

        Value* val = args[0]->codegen(ctx);
        if (!val) return nullptr;

        FunctionType* printfType = FunctionType::get(
            IntegerType::getInt32Ty(ctx.context),
            PointerType::get(Type::getInt8Ty(ctx.context), 0),
            true
        );
        FunctionCallee printfFunc = ctx.module.getOrInsertFunction("printf", printfType);

        Value* fmtStr = ctx.builder.CreateGlobalStringPtr("%f\n", "fmt");
        return ctx.builder.CreateCall(printfFunc, {fmtStr, val});
    }

    Function* CalleeF = ctx.module.getFunction(callee);
    if (!CalleeF) {
        std::cerr << "Error: Unknown function '" << callee << "'\n";
        return nullptr;
    }

    std::vector<Value*> ArgsV;
    for (auto& arg : args) {
        Value* a = arg->codegen(ctx);
        if (!a) return nullptr;
        ArgsV.push_back(a);
    }

    return ctx.builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

// ---------------- Statements ----------------

Value* AssignStmt::codegen(CodegenContext& ctx) {
    Value* val = expr->codegen(ctx);
    if (!val) return nullptr;

    AllocaInst* ptr = nullptr;
    auto it = ctx.NamedValues.find(name);
    if (it != ctx.NamedValues.end()) {
        ptr = it->second;
    } else {
        Function* func = ctx.builder.GetInsertBlock()->getParent();
        IRBuilder<> tmpBuilder(&func->getEntryBlock(), func->getEntryBlock().begin());
        ptr = tmpBuilder.CreateAlloca(val->getType(), nullptr, name.c_str());
        ctx.NamedValues[name] = ptr;
    }

    ctx.builder.CreateStore(val, ptr);
    return val;
}

Value* ReturnStmt::codegen(CodegenContext& ctx) {
    Value* val = expr->codegen(ctx);
    if (!val) return nullptr;

    if (ctx.builder.GetInsertBlock()->getParent()->getName() == "main") {
        val = ctx.builder.CreateFPToSI(val, Type::getInt32Ty(ctx.context), "retint");
    }

    return ctx.builder.CreateRet(val);
}

Value* ExprStmt::codegen(CodegenContext& ctx) {
    return expr->codegen(ctx);
}

Value* WhileStmt::codegen(CodegenContext& ctx) {
    Function* F = ctx.builder.GetInsertBlock()->getParent();
    BasicBlock* CondBB = BasicBlock::Create(ctx.context, "while.cond", F);
    BasicBlock* BodyBB = BasicBlock::Create(ctx.context, "while.body", F);
    BasicBlock* ExitBB = BasicBlock::Create(ctx.context, "while.exit", F);

    ctx.builder.CreateBr(CondBB);
    ctx.builder.SetInsertPoint(CondBB);
    Value* condVal = condition->codegen(ctx);
    if (!condVal) return nullptr;
    condVal = ctx.builder.CreateFCmpONE(condVal, ConstantFP::get(ctx.context, APFloat(0.0)), "cond");
    ctx.builder.CreateCondBr(condVal, BodyBB, ExitBB);

    ctx.builder.SetInsertPoint(BodyBB);
    for (auto& stmt : body)
        stmt->codegen(ctx);
    ctx.builder.CreateBr(CondBB);

    ctx.builder.SetInsertPoint(ExitBB);
    return Constant::getNullValue(Type::getDoubleTy(ctx.context));
}
llvm::Value* IfStmt::codegen(CodegenContext& context) {
    llvm::IRBuilder<>& builder = context.builder;
    llvm::BasicBlock* currBB = builder.GetInsertBlock();
    if (!currBB) {
        std::cerr << "Error: No current insert block.\n";
        return nullptr;
    }
    llvm::Function* function = currBB->getParent();
    if (!function) {
        std::cerr << "Error: No parent function found.\n";
        return nullptr;
    }
llvm::Value* condVal = llvm::ConstantFP::get(context.context, llvm::APFloat(1.0));

    if (!condVal) {
        std::cerr << "Error: Condition codegen failed.\n";
        return nullptr;
    }

    llvm::Value* condBool = builder.CreateFCmpONE(
        condVal,
        llvm::ConstantFP::get(context.context, llvm::APFloat(0.0)),
        "ifcond"
    );

    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context.context, "then", function);
    llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(context.context, "else", function);
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context.context, "ifcont", function);

    builder.CreateCondBr(condBool, thenBB, elseBB);
    builder.SetInsertPoint(thenBB);
    if (!thenStmt || !thenStmt->codegen(context)) {
        std::cerr << "Error: thenStmt codegen failed.\n";
        return nullptr;
    }
    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(mergeBB);
    }

    // ELSE block
    builder.SetInsertPoint(elseBB);
    if (!elseStmt || !elseStmt->codegen(context)) {
        std::cerr << "Error: elseStmt codegen failed.\n";
        return nullptr;
    }
    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(mergeBB);
    }

    // Merge block
    builder.SetInsertPoint(mergeBB);

    return llvm::Constant::getNullValue(llvm::Type::getVoidTy(context.context));
}



llvm::Value* BlockStmt::codegen(CodegenContext& context) {
    llvm::Value* last = nullptr;
    for (auto& stmt : stmts) {
        last = stmt->codegen(context);
    }
    return last;
}
Value* ForStmt::codegen(CodegenContext& ctx) {
    Function* F = ctx.builder.GetInsertBlock()->getParent();
    if (init)
        init->codegen(ctx);

    BasicBlock* CondBB = BasicBlock::Create(ctx.context, "for.cond", F);
    BasicBlock* BodyBB = BasicBlock::Create(ctx.context, "for.body");
    BasicBlock* AfterBB = BasicBlock::Create(ctx.context, "for.end");

    ctx.builder.CreateBr(CondBB);
    ctx.builder.SetInsertPoint(CondBB);
    Value* condVal = cond->codegen(ctx);
    condVal = ctx.builder.CreateFCmpONE(
        condVal, ConstantFP::get(ctx.context, APFloat(0.0)), "forcond");
    ctx.builder.CreateCondBr(condVal, BodyBB, AfterBB);

   
    F->insert(F->end(), BodyBB);
    ctx.builder.SetInsertPoint(BodyBB);
    for (auto& stmt : body)
        stmt->codegen(ctx);
    if (update)
        update->codegen(ctx);
    if (!ctx.builder.GetInsertBlock()->getTerminator())
        ctx.builder.CreateBr(CondBB);

    F->insert(F->end(), AfterBB);
    ctx.builder.SetInsertPoint(AfterBB);

    return Constant::getNullValue(Type::getDoubleTy(ctx.context));
}



// ---------------- Function Definition ----------------

Value* FunctionDef::codegen(CodegenContext& ctx) {
    std::vector<Type*> argTypes(args.size(), Type::getDoubleTy(ctx.context));
    Type* retType = (name == "main") ? Type::getInt32Ty(ctx.context) : Type::getDoubleTy(ctx.context);
    FunctionType* FT = FunctionType::get(retType, argTypes, false);
    Function* F = Function::Create(FT, Function::ExternalLinkage, name, &ctx.module);

    BasicBlock* BB = BasicBlock::Create(ctx.context, "entry", F);
    ctx.builder.SetInsertPoint(BB);

    ctx.NamedValues.clear();
    unsigned idx = 0;
    for (auto& arg : F->args()) {
        arg.setName(args[idx]);
        AllocaInst* alloca = ctx.builder.CreateAlloca(Type::getDoubleTy(ctx.context), nullptr, arg.getName());
        ctx.builder.CreateStore(&arg, alloca);
        ctx.NamedValues[arg.getName().str()] = alloca;
        ++idx;
    }

    for (auto& stmt : body) {
        if (stmt) stmt->codegen(ctx);
    }

    if (name != "main" && !BB->getTerminator()) {
        ctx.builder.CreateRet(ConstantFP::get(ctx.context, APFloat(0.0)));
    }

    verifyFunction(*F);
    return F;
}

Value* RootProgram::codegen(CodegenContext& ctx) {
    FunctionType* mainType = FunctionType::get(Type::getInt32Ty(ctx.context), false);
    Function* mainFunc = Function::Create(mainType, Function::ExternalLinkage, "main", ctx.module);

    BasicBlock* entry = BasicBlock::Create(ctx.context, "entry", mainFunc);
    ctx.builder.SetInsertPoint(entry);
    ctx.NamedValues.clear();

    for (auto& stmt : statements) {
        if (stmt) stmt->codegen(ctx);
    }

    ctx.builder.CreateRet(ConstantInt::get(Type::getInt32Ty(ctx.context), 0));
    verifyFunction(*mainFunc);
    return mainFunc;
}

void FunctionDef::print() {
    std::cout << "Function: " << name << "(";
    for (size_t i = 0; i < args.size(); ++i) {
        std::cout << args[i];
        if (i + 1 < args.size()) std::cout << ", ";
    }
    std::cout << ")\nBody: " << body.size() << " statements\n";
}



