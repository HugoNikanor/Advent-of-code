typedef struct fifo {
	int data[0x10];
	uint8_t read_ptr : 4;
	uint8_t write_ptr : 4;
	int len;
} fifo;

void fifo_put(struct fifo* f, int data) {
	f->data[f->write_ptr++] = data;
}

int fifo_get(struct fifo* f, int* dest) {
	if (f->read_ptr < f->write_ptr) {
		*dest = f->data[f->read_ptr++];
		// no error
		return 0;
	} else {
		return 1;
	}
}

void fifo_reset (struct fifo* f) {
	f->read_ptr  = 0;
	f->write_ptr = 0;

	for (int i = 0; i < 0x10; i++) {
		f->data[i] = 0;
	}

}
