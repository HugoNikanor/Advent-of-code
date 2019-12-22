#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "instr.h"
#include "fifo.h"
#include "process.h"

#define debug 0

void run_asm (proc* p) {
	instr* in;
	int* reg[3];
	int status;

	do {
		/* Fetch next instruction */
		in = (instr*) &proc_mem(p)[p->pc];

#if debug > 1
		print_process(p);
		printf("%2i =%04li:\tin = %i\t%s,\t", p->pid, p->pc, in->op, instr_name[GET_INSTR(in)]);
#endif

		/* Handle adressing modes */
		for (int i = instr_width[GET_INSTR(in)] - 2; i >= 0; i--) {
				switch (PARAM_n(in, i)) {
					case pos: reg[i] = &proc_mem(p)[in->reg[i]]; break;
					case dir: reg[i] = &in->reg[i]; break;
				}
#if debug > 1
				printf("v%i = ", i + 1);
				printf(PARAM_n(in, i) == pos ? "mem[%i]" : "%i", in->reg[i]);
				printf("\t");
#endif
		}

#if debug > 1
				printf("\n");
#endif

		/* Do operation */
		switch (GET_INSTR(in)) {
			case add: *reg[2] = *reg[0] + *reg[1]; break;
			case mul: *reg[2] = *reg[0] * *reg[1]; break;
			case get:
					  status = fifo_get(p->input, reg[0]);
#if debug > 3
					  printf("status = %i\n", status);
#endif
					  if (status != 0) {
						  /* Nothing in input, swap ourselves out
						   * and try for data again later */
						  p->state = waiting;
						  return;
					  }
					  break;

			case put:
					  fifo_put(p->output, *reg[0]);
					  break;
			case jeq:
				  if (*reg[0] != 0) {
					  p->pc = *reg[1];
					  continue;
				  } else break;
			case jnq:
				  if (*reg[0] == 0) {
					  p->pc = *reg[1];
					  continue;
				  } else break;

			case lt: *reg[2] = *reg[0]  < *reg[1]; break;
			case eq: *reg[2] = *reg[0] == *reg[1]; break;
			case halt:
					 p->state = done;
					 return;
			case err:
					 p->state = error;
					 return;
		}

		/* Move PC */
		p->pc += instr_width [GET_INSTR(in)];

	} while (GET_INSTR(in) != halt);

	printf("Code reached end\n");
}

// #include "prog7_test.h"
#include "prog7.h"

int main () {
	// run_asm ();
	tail = &head;

	size_t prog_size = sizeof(prog) / sizeof(int);

	struct fifo fifos[5] = { 0 };

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

						int temp_pos = (a + 5) << (4 * 4)
							| (b + 5) << (4 * 3)
							| (c + 5) << (4 * 2)
							| (d + 5) << 4
							| (e + 5);

						for (int i = 0; i < 5; i++) {
							fifo_reset(&fifos[i]);
						}

						for (int i = 0; i < 5; i++) {
							create_process(prog_size,
									prog, &fifos[i], &fifos[(i+1) % 5]);
						}

						fifo_put(&fifos[0], a + 5);
						fifo_put(&fifos[1], b + 5);
						fifo_put(&fifos[2], c + 5);
						fifo_put(&fifos[3], d + 5);
						fifo_put(&fifos[4], e + 5);

						fifo_put(&fifos[0], 0);

						// int any_runing = 1;
						// while (any_runing) {
						struct process_list_link* link = &head;
						while (head.next != NULL) {
							link = link->next;

							if (! link) {
								link = &head;
								continue;
							}

							if (! link->proc) {
								link->prev->next = link->next;
								link->next->prev = link->prev;

								continue;
							}

							if (link->proc->state == none) {
								link->proc = NULL;
								link->prev->next = link->next;
								if (link->next) {
									link->next->prev = link->prev;
								}

								continue;
							}

							switch (link->proc->state) {
								case running:
									printf("Swaping in already swaped in program\n");

								/* fallthrough */
								case waiting:
								case ready:
#if debug > 0
									printf("Swaping in %i\n", link->proc->pid);
#endif
									link->proc->state = running;
									run_asm (link->proc);
									break;

								case error:
									printf("Process %i exited with error\n",
											link->proc->pid);
								/* fallthrough */
								case done:
									free_process(link->proc);

									break;

								case none:
									break;
							}	

						}	

						head.next = NULL;
						tail = &head;

						int r1;
						int status;
						if ((status = fifo_get(&fifos[0], &r1))) {
							printf("Read error %x\n", status);
						}


						if (r1 > maxval) {
							maxval = r1;
							pos = temp_pos;
						}

					}}}}}

	printf("%x => %i\n",
			pos, maxval);
	
	// 20431 => 273814

	return 0;
}
