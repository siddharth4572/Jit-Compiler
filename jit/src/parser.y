%define parse.error verbose
%define parse.trace

%code requires {
    #include "ast.hpp"
    #include <memory>
    #include <vector>
    #include <string>
}

%{
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

#include "ast.hpp"

extern int yylex();
void yyerror(const char *s);
FunctionDef* rootFunction = nullptr;
Expr* rootExpr = nullptr;
Stmt* rootStmt = nullptr;
extern RootProgram* rootProgram;

extern llvm::LLVMContext TheContext;
extern llvm::IRBuilder<> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value*> NamedValues;

// Helper function to convert statement list to BlockStmt
Stmt* createBlockStmt(std::vector<Stmt*>* stmtList) {
    std::vector<std::unique_ptr<Stmt>> stmts;
    for (auto* s : *stmtList) {
        if (s) stmts.emplace_back(std::unique_ptr<Stmt>(s));
    }
    delete stmtList;
    return new BlockStmt(std::move(stmts));
}
%}

%union {
    int num;
    char* str;
    Expr* expr;
    Stmt* stmt;
    FunctionDef* func;
    BlockStmt* block;
    std::vector<FunctionDef*>* funcList;
    std::vector<Expr*>* exprList;
    std::vector<std::string>* strList;
    std::vector<Stmt*>* stmtList;
    RootProgram* program;
}

%start program
%token <str> IDENTIFIER
%token <num> NUMBER
%token DEF RETURN WHILE IF ELSE FOR
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMICOLON ASSIGN
%token '<' '>' EQ NE

%type <stmtList> stmt_list
%type <stmt> stmt
%type <expr> expr
%type <exprList> arg_list
%type <strList> param_list
%type <program> program
%type <funcList> function_list
%type <func> func
%type <stmt> for_initopt
%type <expr> for_condopt
%type <stmt> for_updateopt

%left '+' '-'
%left '*' '/'
%left '<' '>' EQ NE


%%

program:
      /* empty */ { rootProgram = nullptr; }
    | func {
        std::vector<std::unique_ptr<Stmt>> stmts;
        stmts.emplace_back(std::unique_ptr<Stmt>(rootFunction));
        rootProgram = new RootProgram(std::move(stmts));
    }
    | stmt_list {
        std::vector<std::unique_ptr<Stmt>> stmts;
        for (auto* s : *$1) stmts.emplace_back(std::unique_ptr<Stmt>(s));
        delete $1;
        rootProgram = new RootProgram(std::move(stmts));
    }
    | function_list {
        std::vector<std::unique_ptr<Stmt>> stmts;
        for (auto* f : *$1)
            stmts.emplace_back(std::unique_ptr<Stmt>(f));
        delete $1;
        rootProgram = new RootProgram(std::move(stmts));
    }
;

function_list:
      func {
        $$ = new std::vector<FunctionDef*>();
        $$->push_back($1);
    }
    | function_list func {
        $1->push_back($2);
        $$ = $1;
    }
;

func:
    DEF IDENTIFIER LPAREN param_list RPAREN LBRACE stmt_list RBRACE {
        std::vector<std::unique_ptr<Stmt>> stmts;
        for (auto* s : *$7) stmts.emplace_back(std::unique_ptr<Stmt>(s));
        delete $7;
        rootFunction = new FunctionDef(std::string($2), std::move(*$4), std::move(stmts));
        delete $4;
        free($2);
    }
;

stmt:
      IDENTIFIER ASSIGN expr SEMICOLON {
        if (!$3) {
            std::cerr << "[parser.y] Error: assignment expression is null\n";
            $$ = nullptr;
        } else {
            $$ = new AssignStmt(std::string($1), std::unique_ptr<Expr>($3));
        }
        free($1);
    }
    | LBRACE stmt_list RBRACE {
    std::vector<std::unique_ptr<Stmt>> stmts;
    for (auto* s : *$2) stmts.emplace_back(std::unique_ptr<Stmt>(s));
    delete $2;
    $$ = new BlockStmt(std::move(stmts));
    } 
    | RETURN expr SEMICOLON {
        $$ = new ReturnStmt(std::unique_ptr<Expr>($2));
    }
    | expr SEMICOLON {
        $$ = new ExprStmt(std::unique_ptr<Expr>($1));
    }
    | WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE {
        std::vector<std::unique_ptr<Stmt>> stmts;
        for (auto* s : *$6) stmts.emplace_back(std::unique_ptr<Stmt>(s));
        delete $6;
        $$ = new WhileStmt(std::unique_ptr<Expr>($3), std::move(stmts));
    }
    | IF LPAREN expr RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE {
        Stmt* thenBlock = createBlockStmt($6);
        Stmt* elseBlock = createBlockStmt($10);
        $$ = new IfStmt(
            std::unique_ptr<Expr>($3),
            std::unique_ptr<Stmt>(thenBlock),
            std::unique_ptr<Stmt>(elseBlock)
        );
    }
    | FOR LPAREN for_initopt SEMICOLON for_condopt SEMICOLON for_updateopt RPAREN LBRACE stmt_list RBRACE {
        std::vector<std::unique_ptr<Stmt>> bodyStmts;
        for (auto* s : *$10) bodyStmts.emplace_back(std::unique_ptr<Stmt>(s));
        delete $10;
        $$ = new ForStmt(
            $3 ? std::unique_ptr<Stmt>($3) : nullptr,
            $5 ? std::unique_ptr<Expr>($5) : nullptr,
            $7 ? std::unique_ptr<Stmt>($7) : nullptr,
            std::move(bodyStmts)
        );
    }
;

for_initopt:
      IDENTIFIER ASSIGN expr {
        $$ = new AssignStmt(std::string($1), std::unique_ptr<Expr>($3));
        free($1);
    }
    | /* empty */ { $$ = nullptr; }
;

for_updateopt:
      IDENTIFIER ASSIGN expr {
        $$ = new AssignStmt(std::string($1), std::unique_ptr<Expr>($3));
        free($1);
    }
    | /* empty */ { $$ = nullptr; }
;

for_condopt:
      expr { $$ = $1; }
    | /* empty */ { $$ = nullptr; }
;

stmt_list:
      stmt {
        $$ = new std::vector<Stmt*>();
        if ($1) $$->push_back($1);
    }
    | stmt_list stmt {
        if ($2) $1->push_back($2);
        $$ = $1;
    }
;

expr:
      expr '+' expr { $$ = new BinaryExpr("+", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr '-' expr { $$ = new BinaryExpr("-", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr '*' expr { $$ = new BinaryExpr("*", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr '/' expr { $$ = new BinaryExpr("/", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr '<' expr { $$ = new BinaryExpr("<", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr '>' expr { $$ = new BinaryExpr(">", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr EQ expr  { $$ = new BinaryExpr("==", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | expr NE expr  { $$ = new BinaryExpr("!=", std::unique_ptr<Expr>($1), std::unique_ptr<Expr>($3)); }
    | IDENTIFIER LPAREN arg_list RPAREN {
        std::vector<std::unique_ptr<Expr>> args;
        for (auto* arg : *$3) args.emplace_back(std::unique_ptr<Expr>(arg));
        delete $3;
        $$ = new FunctionCall(std::string($1), std::move(args));
        free($1);
    }
    | IDENTIFIER {
        $$ = new Variable(std::string($1));
        free($1);
    }
    | NUMBER {
        $$ = new Number($1);
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
;


arg_list:
      /* empty */ { $$ = new std::vector<Expr*>(); }
    | expr {
        $$ = new std::vector<Expr*>();
        $$->push_back($1);
    }
    | arg_list COMMA expr {
        $1->push_back($3);
        $$ = $1;
    }
;


param_list:
      /* empty */ { $$ = new std::vector<std::string>(); }
    | IDENTIFIER {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string($1));
        free($1);
    }
    | param_list COMMA IDENTIFIER {
        $1->push_back(std::string($3));
        free($3);
        $$ = $1;
    }
;

%%


