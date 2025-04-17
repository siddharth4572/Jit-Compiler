#include <cstdio>
#include <cstdlib>
#include "parser.hpp"  

extern FILE* yyin;

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

