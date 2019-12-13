#include "fifo.h"

void fifo_put(struct pipe* f_, int64_t data) {
	struct fifo* f = (struct fifo*) f_;
	f->len++;
	f->data[f->write_ptr++] = data;
}

int fifo_get(struct pipe* f_, int64_t* dest) {
	struct fifo* f = (struct fifo*) f_;
	if (f->read_ptr < f->write_ptr) {
		*dest = f->data[f->read_ptr++];
		// no error
		--f->len;
		return 0;
	} else {
		return f->write_ptr << 4 | f->read_ptr;
	}
}

void fifo_flush (struct pipe* f_) {
	struct fifo* f = (struct fifo*) f_;
	f->read_ptr  = 0;
	f->write_ptr = 0;
	f->len = 0;

	for (int i = 0; i < 0x10; i++) {
		f->data[i] = 0;
	}
}

void make_fifo(struct fifo* mem) {
	mem->proc.get = &fifo_get;
	mem->proc.put = &fifo_put;
	mem->proc.flush = &fifo_flush;
}
