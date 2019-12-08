typedef enum opcode {
	add = 1, // r1, r2, r3
	mul = 2, // r1, r2, r3
	get = 3,
	put = 4,
	jeq = 5, // jump if true
	jnq = 6, // jump if false
	lt  = 7,  // less than
	eq  = 8,  // equal
	halt = 99, //
} opcode;

typedef enum param_mode {
	pos = 0, // position in memmory
	dir = 1, // direct value
} param_mode;

int instr_width [halt + 1] = {
	[add] = 4,
	[mul] = 4,
	[get] = 2,
	[put] = 2,
	[jeq] = 3,
	[jnq] = 3,
	[lt]  = 4,
	[eq]  = 4,
	[halt] = 1,
};

char* instr_name [halt + 1] = {
	[add] = "add",
	[mul] = "mul",
	[get] = "get",
	[put] = "put",
	[jeq] = "jeq",
	[jnq] = "jnq",
	[lt]  = "lt",
	[eq]  = "eq",
	[halt] = "halt",
};

typedef struct instr {
	opcode op;
	int reg[0];
} instr;

int int_pow(int base, int exp) {
	int t = 1;
	for (int i = 0; i < exp; i++)
		t *= base;
	return t;
}

#define PARAM_1(in)  (((in->op) / 100  ) % 10)
#define PARAM_2(in)  (((in->op) / 1000 ) % 10)
#define PARAM_3(in)  (((in->op) / 10000) % 10)
#define PARAM_n(in, n) (((in->op) / (100 * int_pow(10, (n)))) % 10)
#define GET_INSTR(in) ((in->op) % 100)
