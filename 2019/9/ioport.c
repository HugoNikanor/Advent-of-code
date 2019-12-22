#include "ioport.h"

#include <stdio.h>

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


