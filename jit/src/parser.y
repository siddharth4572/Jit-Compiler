%{
#include <iostream>
#include <memory>
#include <string>
#include "ast.hpp"

int yylex();
void yyerror(const char *s);

std::unique_ptr<ast::Expr> root;
%}

%union {
    int num;
    char* id;
    void* expr;
}

%token <num> NUM
%token <id> IDENT
%token LET
%token FUNC
%token RETURN

%type <expr> expr
%type <expr> stmt

%start program

%%

program
    : stmt              { root.reset((ast::Expr*)$1); }
    ;

stmt
    : LET IDENT '=' expr ';' {
        $$ = new ast::VarAssign($2, (ast::Expr*)$4);
        free($2);
    }
    | expr ';'          { $$ = $1; }
    ;

expr
    : expr '+' expr     { $$ = new ast::BinaryOp("+", (ast::Expr*)$1, (ast::Expr*)$3); }
    | expr '-' expr     { $$ = new ast::BinaryOp("-", (ast::Expr*)$1, (ast::Expr*)$3); }
    | expr '*' expr     { $$ = new ast::BinaryOp("*", (ast::Expr*)$1, (ast::Expr*)$3); }
    | expr '/' expr     { $$ = new ast::BinaryOp("/", (ast::Expr*)$1, (ast::Expr*)$3); }
    | '(' expr ')'      { $$ = $2; }
    | NUM               { $$ = new ast::Number($1); }
    | IDENT             { $$ = new ast::Variable($1); free($1); }
    ;

%%

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << std::endl;
}