#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "../include/ast.hpp"

extern int yyparse();
extern std::unique_ptr<ast::Expr> root;
extern void restart_lexer(const char*);

std::unordered_map<std::string, int> variables;

int main() {
    std::string line;
    std::cout << ">>> JIT REPL with AST Evaluation <<<\n";
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        restart_lexer(line.c_str());
        if (yyparse() == 0 && root) {
            try {
                int result = root->evaluate(variables);
                std::cout << "=> " << result << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}