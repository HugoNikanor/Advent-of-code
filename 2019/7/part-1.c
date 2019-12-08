#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "instr.h"
#include "fifo.h"

#define debug 0

#define PAGES (0x100 - 1)

typedef enum process_states {
	none = 0,
	ready, running, waiting, done,
	error,
} process_state;

char* state_name[] = {
	[none]    = "none",
	[ready]   = "ready",
	[running] = "running",
	[waiting] = "waiting",
	[done]    = "done",
	[error]   = "error",
};

typedef struct proc {
	int pid;
	size_t pc;
	size_t size;
	process_state state;
	fifo *input, *output;
} proc;


/* primary memory */
struct page { 
	proc proc;
	int mem[0x1000];
} mem[PAGES];

struct process_list_link {
	struct proc* proc;
	struct process_list_link* next;
} head, *tail, links[0x100 - 1];

/* Implicit overflow */
uint8_t current_link_slot = 0;

int current_pid = 0;
int current_page = 0;

int* proc_mem(proc* p) {
	return ((int*) (((uint8_t*) p) + sizeof(struct proc)));
}

void print_process (proc* p) {
	printf("process = {\n\tpid = %i\n\tpc = %li\n\tstate = %s\n\tmem = [%li]\n\t\t",
			p->pid, p->pc, state_name[p->state], p->size);
	for (size_t i = 0; i < p->size; i++) {
		if (i == p->pc) {
			printf("[%i], ", proc_mem(p)[i]);
		} else {
			printf("%i, ", proc_mem(p)[i]);
		}
	}
	printf("\n\t}\n}\n");
}


struct proc* find_page () {
	struct proc* proc = NULL;

	for (uint8_t i = current_page + 1; i != current_page; i++) {
		if (mem[i].proc.state == none) {
			proc = (struct proc*) &mem[i];
			current_page = i;
			break;
		}
	}

	return proc;
}

struct process_list_link* get_link () {
	for (uint8_t i = current_link_slot + 1; i != current_link_slot; i++) {
		if (! links[i].proc || links[i].proc->state == none) {
			return &links[i];
		}
	}
	return NULL;
}

struct process_list_link* find_process(int pid) {
	for (struct process_list_link* link = head.next; link != NULL; link = link->next) {
		if (link->proc && link->proc->pid == pid) {
			return link;
		}
	}

	/* Process not loaded, searching link table */
	printf("/* Process not loaded, searching link table */\n");

	for (uint8_t i = 0; i < 0x100 - 1; i++) {
		if (links[i].proc->pid == pid)
			return &links[i];
	}

	return NULL;
}

struct proc*
create_process (int prog_size, int* prog,
		fifo* input, fifo* output) {

	struct proc* proc = find_page();

	if (proc == NULL) {
		printf("FATAL: no empty pages\n");
		exit (1);
	}

	proc->state = ready;

	proc->input = input;
	proc->output = output;

	proc->size = prog_size;
	proc->pc = 0;

	/* copy program into memmory */
	memcpy (proc_mem(proc), prog, prog_size * sizeof(int));
	proc->pid = current_pid++;

	struct process_list_link* link = get_link();
	if (link == NULL) {
		printf("FATAL: no empty links\n");
		exit (1);
	}

	link->next = NULL;
	link->proc = proc;
	tail->next = link;
	tail = link;

	return proc;
}

int free_process (struct proc* p) {
	p->state = none;
	return 0;
}

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

#include "prog7_test.h"

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

						printf("n = %x\n", temp_pos);


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
						struct process_list_link* prev = &head;
						struct process_list_link* link = head.next;
						while (head.next != NULL) {
							if (! link->proc || link->proc->state == none) {
								link->proc = NULL;
								if (link == tail) {
									tail = prev;
								}
								prev->next = link->next;
								link = link->next;
							}

							switch (link->proc->state) {
								case running:
									printf("Swaping in already swaped in program\n");

								/* fallthrough */
								case ready:
								case waiting:
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

							prev = link;
							link = link->next;
							if (link == NULL) {
								prev = &head;
								link = head.next;
							}
						}	

						int r1;
						fifo_get(&fifos[4], &r1);

						printf("r1 = %i\n", r1);

#if 0
						for (struct process_list_link *link = &head;
								link != NULL;
								link = link->next) {
							if (link->proc)
								free_process(link->proc);
						}
#endif

						head.next = NULL;
						tail = &head;

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
