#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "instr.h"
#include "fifo.h"

// #define debug

void run_asm (int* mem, fifo* f) {
	int* pc = mem;
	instr* in = (instr*) pc;

	int* reg[3];

	do {
		/* Fetch next instruction */
		in = (instr*) pc;

#ifdef debug
		printf("=%04li:\t%s,\t", pc - mem, instr_name[GET_INSTR(in)]);
#endif

		/* Handle adressing modes */
		for (int i = instr_width[GET_INSTR(in)] - 2; i >= 0; i--) {
				switch (PARAM_n(in, i)) {
					case pos: reg[i] = &mem[in->reg[i]]; break;
					case dir: reg[i] = &in->reg[i]; break;
				}
#ifdef debug
				printf("v%i = ", i + 1);
				printf(PARAM_n(in, i) == pos ? "mem[%i]" : "%i", *reg[i]);
				printf("\t");
#endif
		}

		/* Do operation */
		switch (GET_INSTR(in)) {
			case add: *reg[2] = *reg[0] + *reg[1]; break;
			case mul: *reg[2] = *reg[0] * *reg[1]; break;
			case get: *reg[0] = fifo_get(f); break;
			case put: fifo_put(f, *reg[0]); break;
			case jeq:
				  if (*reg[0] != 0) {
					  pc = mem;
					  pc += *reg[1];
					  continue;
				  } else break;
			case jnq:
				  if (*reg[0] == 0) {
					  pc = mem;
					  pc += *reg[1];
					  continue;
				  } else break;

			case lt: *reg[2] = *reg[9]  < *reg[1]; break;
			case eq: *reg[2] = *reg[9] == *reg[1]; break;
			case halt: return;
		}

		/* Move PC */
		pc += instr_width [GET_INSTR(in)];

	} while (GET_INSTR(in) != halt);
}

#include "prog7.h"

/* primary memory */
int mem[0x10000];

int main () {
	// run_asm ();

	size_t size = sizeof(prog) / sizeof(int);

	struct fifo fifo1 = { 0 };

	int maxval = 0;
	int pos = 0;

	for (int a = 0; a < 5; a++) {
		for (int b = 0; b < 5; b++) {
			if (b == a) continue;
			for (int c = 0; c < 5; c++) {
				if (c == b || c == a) continue;
				for (int d = 0; d < 5; d++) {
					if (d == c || d == b || d == a) continue;
					for (int e = 0; e < 5; e++) {
						if (e == d || e == c || e == b || e == a) continue;
						fifo_reset(&fifo1);

						fifo_put(&fifo1, a);
						fifo_put(&fifo1, 0);
						fifo_put(&fifo1, b);

						// { a, 0, b }

						memcpy(mem,            prog, size);
						run_asm (mem,            &fifo1);


						fifo_put(&fifo1, c);

						// { b, out, c }

						memcpy(mem,            prog, size);
						run_asm (mem,            &fifo1);

						fifo_put(&fifo1, d);

						// { c, out2, d }

						memcpy(mem,            prog, size);
						run_asm (mem,            &fifo1);

						fifo_put(&fifo1, e);

						// { d, out3, e }

						memcpy(mem,            prog, size);
						run_asm (mem,            &fifo1);

						// { e, out4 }
						
						memcpy(mem,            prog, size);
						run_asm (mem,            &fifo1);

						// { out5 }

						int r1 = fifo_get(&fifo1);

						if (r1 > maxval) {
							maxval = r1;
							pos = a << (4 * 4)
								| b << (4 * 3)
								| c << (4 * 2)
								| d << 4
								| e;
						}

					}}}}}

	printf("%x => %i\n",
			pos, maxval);
	
	// 20431 => 273814

	return 0;
}
