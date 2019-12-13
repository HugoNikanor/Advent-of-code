#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

struct pipe;

struct pipe_proc {
	void (*put)(struct pipe*, int64_t data);
	int (*get)(struct pipe*, int64_t* dest);
	void (*flush)(struct pipe*);
};

struct pipe {
	struct pipe_proc proc;
};

struct fifo {
	struct pipe_proc proc;
	int data[0x100];
	uint8_t read_ptr;
	uint8_t write_ptr;
	int len;
};

void fifo_put(struct pipe* f_, int64_t data);
int fifo_get(struct pipe* f_, int64_t* dest);
void fifo_flush (struct pipe* f_);

void make_fifo(struct fifo* mem);


#endif /* FIFO_H */
