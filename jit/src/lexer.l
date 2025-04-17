%{
#include "parser.hpp"
%}


%option noyywrap
%%


[0-9]+              { yylval.num = atoi(yytext); return NUM; }
"let"               { return LET; }
"func"              { return FUNC; }
"return"            { return RETURN; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.id = strdup(yytext); return IDENT; }
[ \t\r\n]+       { /* skip whitespace */ }
.                   { return yytext[0]; }

%%
