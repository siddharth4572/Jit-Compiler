%option noyywrap

%{
#include "parser.hpp"
#include <cstdlib>
#include <cstring>
%}

DIGIT   [0-9]+
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%
"def"            { return DEF; }
"return"         { return RETURN; }
"while"          { return WHILE; }
"if"             { return IF; }
"else"           { return ELSE; }
"for"            { return FOR; }

"=="             { return EQ; }
"!="             { return NE; }
"<"              { return '<'; }
">"              { return '>'; }

"="              { return ASSIGN; }
"("              { return LPAREN; }
")"              { return RPAREN; }
"{"              { return LBRACE; }
"}"              { return RBRACE; }
","              { return COMMA; }
";"              { return SEMICOLON; }
"+"              { return '+'; }
"-"              { return '-'; }
"*"              { return '*'; }
"/"              { return '/'; }

{DIGIT}          { yylval.num = atoi(yytext); return NUMBER; }
{ID}             { yylval.str = strdup(yytext); return IDENTIFIER; }

[ \t\r]+         { }
\n               { }
. {
    fprintf(stderr, "Lexer error: unexpected char '%s' (code %d)\n", yytext, yytext[0]);
    return yytext[0];
}

%% 

