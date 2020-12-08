#include "types.h"

const char* stringify_op(enum opcode op) {
	switch (op) {
		case nop: return "nop";
		case acc: return "acc";
		case jmp: return "jmp";
	}
}

