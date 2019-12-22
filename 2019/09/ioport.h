#ifndef IOPORT_H
#define IOPORT_H

#include "fifo.h"

void port_put(struct pipe* p, int64_t data);
int port_get(struct pipe* p, int64_t* dest);

void make_ioport (struct pipe* mem);


#endif /* IOPORT_H */
