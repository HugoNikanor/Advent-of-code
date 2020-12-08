#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "types.h"

int eval(struct operation* operations, int* ret) {
	int ac = 0;
	int pc = 0;
	int has_corrected = 0;

	while (1) {
		struct operation* op = & operations[pc];

		/*
		printf("[%d] %d, %s, %d\n",
				ac, pc, stringify_op(op->op),
				op->operand);
		*/


		if (op->visited) { *ret = ac; return -1; }
		op->visited = 1;

		enum opcode oc = op->op;

		/*
		 * On NOP or JMP
		 * - continue as if nothing, and
		 * - assume corruption
		 */

		if (! has_corrected) switch (oc) {
			case nop:
			case jmp:
				if (0 == fork()) {
					/* child, assume corruption */
					switch (oc) {
						case nop: oc = jmp; break;
						case jmp: oc = nop; break;
					}
					has_corrected = 1;
				} else {
					/* parent, continue as normal */
				}
				break;
			default: ;
		};

		switch (oc) {
			case acc:
				ac += op->operand;
			case nop:
				++pc;
				continue;
			case jmp:
				pc += op->operand;
				continue;
			case end:
				*ret = ac;
				return 0;
		}
	}
}
