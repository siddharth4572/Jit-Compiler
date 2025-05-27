#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "parser.hpp"

extern FILE* yyin;

void yyerror(const char* s) {
    std::cerr << "Parser error: " << s << std::endl;
}
void restart_lexer(const char* input) {
    static FILE* tmp_input = nullptr;

    if (tmp_input) {
        fclose(tmp_input);
    }

    tmp_input = tmpfile();
    if (!tmp_input) {
        perror("tmpfile");
        exit(EXIT_FAILURE);
    }

    fputs(input, tmp_input);
    rewind(tmp_input);
    yyin = tmp_input;
}

