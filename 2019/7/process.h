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
	struct process_list_link* prev;
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

	// for (uint8_t i = current_page + 1; i != current_page; i++) {
	for (int i = 0; i < PAGES; i++) {
		if (mem[i].proc.state == none) {
			proc = (struct proc*) &mem[i];
			// current_page = i;
			break;
		}
	}

	return proc;
}

struct process_list_link* get_link () {
	// for (uint8_t i = current_link_slot + 1; i != current_link_slot; i++) {
	for (int i = 0; i < 0x100; i++) {
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
	link->prev = tail;

	tail->next = link;

	tail = link;

	return proc;
}

int free_process (struct proc* p) {
	p->state = none;
	return 0;
}

