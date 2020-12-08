#include <stdio.h>
#include <string.h>
#include "types.h"
#include "eval.h"

extern int yylex();
extern int yylval;

int main() {

	struct operation operations[1000];
	memset(operations, 0, 1000 * sizeof(struct operation));
	int current = 0;

	/* TODO replace with a proper parser. Currently nothing validates
	 * that there is one instruction per line, and that each line is
	 * one operation, and one operand */
	int tok;
	while ((tok = yylex())) {
		switch (tok) {
			case OP:
				operations[current].op = yylval;
				break;
			case NUM:
				operations[current].operand = yylval;
				++current;
		}
	}

	printf("Got %d lines\n", current);

	int ret;
	int status = eval(operations, &ret);
	if (status == 0) {
		printf("end! ac = %d\n", ret);
	}

}

