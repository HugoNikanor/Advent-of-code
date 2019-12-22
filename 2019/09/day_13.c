#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "instr.h"
#include "fifo.h"
#include "process.h"
#include "ioport.h"
#include "gpu_port.h"

#define debug 0

void run_asm (proc* p) {
	instr* in;
	int64_t* reg[3];
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
					case rel: reg[i] = &proc_mem(p)[p->rel_base + in->reg[i]]; break;
				}
#if debug > 1
				printf("v%i = ", i + 1);
				switch (PARAM_n(in, i)) {
					case pos: printf("mem[%li]", in->reg[i]); break;
					case dir: printf("#%li", in->reg[i]); break;
					case rel: printf("(%li + #%li)", p->rel_base, in->reg[i]);
				}
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
					  status = p->input->proc.get(p->input, reg[0]);
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
					  p->output->proc.put(p->output, *reg[0]);
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
			case srb: p->rel_base += *reg[0]; break;
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
// #include "prog7.h"
// #include "quine.h"
// #include "16digit.h"
#include "prog/prog13.h"

int main () {
	// run_asm ();
	tail = &head;

	size_t prog_size = sizeof(prog) / sizeof(int64_t);

	// struct fifo fifos[2] = { 0 };
	struct pipe iopipe;
	make_ioport (&iopipe);

	struct gpu_port gpu_port = { 0 };
	make_gpu_port (&gpu_port);

	// create_process(prog_size, prog, (struct pipe*) &iopipe, (struct pipe*) &gpu_port);
	create_process(prog_size, prog, (struct pipe*) &gpu_port, (struct pipe*) &gpu_port);

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

	// int64_t dest;
	// gpu_port.proc.get((struct pipe*) &gpu_port, &dest);
	// printf(" tiles = %li\n", dest); 

	head.next = NULL;
	tail = &head;

	printf("\n");

	return 0;
}
