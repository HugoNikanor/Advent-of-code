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

void port_put(struct pipe* p, int64_t data) {
	(void) p;
	printf("%li ", data);
}

int port_get(struct pipe* p, int64_t* dest) {
	(void) p;
	printf("> ");
	int ret;
	ret = scanf("%li", dest);

	if (ret) return 0;
	return 1;
}

void make_ioport (struct pipe* mem) {
	mem->proc.put = port_put;
	mem->proc.get = port_get;
}
