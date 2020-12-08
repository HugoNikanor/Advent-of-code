#include <stdio.h>
#include "types.h"

int eval(struct operation* operations) {
	int ac = 0;
	int pc = 0;

	while (1) {
		struct operation* op = & operations[pc];

		printf("[%d] %d, %s, %d\n",
				ac, pc, stringify_op(op->op),
				op->operand);


		if (op->visited) { return ac; }
		op->visited = 1;

		switch (op->op) {
			case nop:
				++pc;
				continue;
			case acc:
				ac += op->operand;
				++pc;
				continue;
			case jmp:
				pc += op->operand;
				continue;
		}
	}
}
