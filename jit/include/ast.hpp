#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>

namespace ast {

class Expr {
public:
    virtual ~Expr() = default;
    virtual int evaluate(std::unordered_map<std::string, int>& variables) = 0;
};

class Number : public Expr {
    int value;
public:
    Number(int val) : value(val) {}
    int evaluate(std::unordered_map<std::string, int>&) override { return value; }
};

class Variable : public Expr {
    std::string name;
public:
    Variable(const std::string& n) : name(n) {}
    int evaluate(std::unordered_map<std::string, int>& variables) override {
        if (variables.find(name) == variables.end()) throw std::runtime_error("Undefined variable: " + name);
        return variables[name];
    }
};

class BinaryOp : public Expr {
    std::string op;
    std::unique_ptr<Expr> lhs, rhs;
public:
    BinaryOp(const std::string& o, Expr* l, Expr* r) : op(o), lhs(l), rhs(r) {}
    int evaluate(std::unordered_map<std::string, int>& variables) override {
        int a = lhs->evaluate(variables);
        int b = rhs->evaluate(variables);
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") return a / b;
        throw std::runtime_error("Unknown operator: " + op);
    }
};

class VarAssign : public Expr {
    std::string name;
    std::unique_ptr<Expr> value;
public:
    VarAssign(const std::string& n, Expr* v) : name(n), value(v) {}
    int evaluate(std::unordered_map<std::string, int>& variables) override {
        int val = value->evaluate(variables);
        variables[name] = val;
        return val;
    }
};

} // namespace ast

#endif // AST_HPP