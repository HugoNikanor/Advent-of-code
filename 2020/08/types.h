#ifndef TYPES_H
#define TYPES_H


enum opcode {
	end,
	nop,
	acc,
	jmp,
};


struct operation {
	enum opcode op;
	int operand;
	int visited;
};

enum yytoktype {
	OP = 1,
	NUM,
};

const char* stringify_op(enum opcode op);

#endif /* TYPES_H */
