typedef struct fifo {
	int data[0x10];
	size_t read, write;
	int len;
} fifo;

void fifo_put(struct fifo* f, int data) {
	f->data[f->write++] = data;
}

int fifo_get(struct fifo* f) {
	return f->data[f->read++];
}

void fifo_reset (struct fifo* f) {
	f->read = 0;
	f->write = 0;
}
