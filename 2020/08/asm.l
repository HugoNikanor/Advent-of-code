%{
#include <stdio.h>
#include "types.h"


int yylval;

%}

%%

nop { yylval = nop; return OP; }
acc { yylval = acc; return OP; }
jmp { yylval = jmp; return OP; }

[+-][0-9]+	{ yylval = atoi(yytext); return NUM; }

[ \t\n]+	/* eat whitespace */

.			printf("Unknown character: %s\n", yytext);
%%

