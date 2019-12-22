#ifndef GPU_PORT_H
#define GPU_PORT_H

#include "fifo.h"

enum state {
	next_x, next_y, next_tile
};

enum tiles {
	empty_tile = 0,
	wall_tile,
	block_tile,
	horizontal_paddle_tile,
	ball_tile,
};

enum input_state {
	left = -1, neutral = 0, right = 1,
	no_input
};

struct gpu_port {
	struct pipe_proc proc;
	int64_t x, y;
	enum state next;
	uint64_t score;
	enum input_state input;
	enum tiles screen[21][38];
};

void gpu_port_put(struct pipe* p, int64_t data);
int gpu_port_get(struct pipe* p, int64_t* dest);

void make_gpu_port (struct gpu_port* mem);

#endif /* GPU_PORT_H */
