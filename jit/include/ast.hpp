#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <memory>
#include "llvm/IR/Value.h"

class CodegenContext;

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// ---------------- Expressions ----------------
class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
    virtual llvm::Value* codegen(CodegenContext&) = 0;
};

class Number : public Expr {
public:
    double value;
    Number(double v) : value(v) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class Variable : public Expr {
public:
    std::string name;
    Variable(std::string n) : name(std::move(n)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class BinaryExpr : public Expr {
public:
    std::string op;
    std::unique_ptr<Expr> lhs, rhs;
    BinaryExpr(std::string o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(std::move(o)), lhs(std::move(l)), rhs(std::move(r)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class FunctionCall : public Expr {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;
    FunctionCall(std::string c, std::vector<std::unique_ptr<Expr>> a)
        : callee(std::move(c)), args(std::move(a)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

// ---------------- Statements ----------------
class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
    virtual llvm::Value* codegen(CodegenContext&) = 0;
};

class AssignStmt : public Stmt {
public:
    std::string name;
    std::unique_ptr<Expr> expr;
    AssignStmt(std::string n, std::unique_ptr<Expr> e)
        : name(std::move(n)), expr(std::move(e)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> expr;
    ReturnStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> stmts;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts)
        : stmts(std::move(stmts)) {}

    llvm::Value* codegen(CodegenContext& context) override;
};


class ExprStmt : public Stmt {
public:
    std::unique_ptr<Expr> expr;
    ExprStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;
    WhileStmt(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Stmt>> b)
        : condition(std::move(cond)), body(std::move(b)) {}
    llvm::Value* codegen(CodegenContext&) override;
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Stmt> thenStmt;
    std::unique_ptr<Stmt> elseStmt; // always non-null

    IfStmt(std::unique_ptr<Expr> c, std::unique_ptr<Stmt> t, std::unique_ptr<Stmt> e)
      : cond(std::move(c)), thenStmt(std::move(t)), elseStmt(std::move(e)) {}

    llvm::Value* codegen(CodegenContext& context) override;
};


class ForStmt : public Stmt {
public:
    std::unique_ptr<Stmt> init;
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Stmt> update;
    std::vector<std::unique_ptr<Stmt>> body;
    ForStmt(std::unique_ptr<Stmt> i, std::unique_ptr<Expr> c, std::unique_ptr<Stmt> u, std::vector<std::unique_ptr<Stmt>> b)
        : init(std::move(i)), cond(std::move(c)), update(std::move(u)), body(std::move(b)) {}
    llvm::Value* codegen(CodegenContext& context) override;
};

class FunctionDef : public Stmt {
public:
    std::string name;
    std::vector<std::string> args;
    std::vector<std::unique_ptr<Stmt>> body;
    FunctionDef(std::string n, std::vector<std::string> a, std::vector<std::unique_ptr<Stmt>> b)
        : name(std::move(n)), args(std::move(a)), body(std::move(b)) {}
    llvm::Value* codegen(CodegenContext&) override;
    void print();
};

class RootProgram : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    RootProgram(std::vector<std::unique_ptr<Stmt>> stmts)
        : statements(std::move(stmts)) {}
    llvm::Value* codegen(CodegenContext& ctx);
};

// Declare global variable (defined in exactly one .cpp)
extern RootProgram* rootProgram;

#endif  // AST_HPP

