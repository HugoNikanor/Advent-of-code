#include "gpu_port.h"
#include <stdio.h>
#include <pthread.h>

#include <SDL.h>
#include <SDL_ttf.h>

void gpu_port_put(struct pipe* p_, int64_t data) {
	struct gpu_port* p = (struct gpu_port*) p_;

	switch (p->next) {
		case next_x:
			p->x = data;
			p->next = next_y;
			break;
		case next_y:
			p->y = data;
			p->next = next_tile;
			break;
		case next_tile:
			p->next = next_x;
			if (p->x == -1 && p->y == 0) {
				p->score = data;
			} else {
				p->screen[p->y][p->x] = (enum tiles) data;
			}
			/* Actually paint here */
			// printf("(%3li, %3li) => %i\n", p->x, p->y, p->tile);
			break;
	}

}

int gpu_port_get(struct pipe* p, int64_t* dest) {
	*dest = (int64_t) ((struct gpu_port*) p)->input;
	if (*dest != no_input) {
		((struct gpu_port*) p)->input = no_input;
		return 0;
	} else {
		return 1;
	}
}

SDL_Window* window;
SDL_Renderer* renderer;

void* init_sdl (void* mem) {

	struct gpu_port* p = (struct gpu_port*) mem;

	SDL_Init(SDL_INIT_VIDEO);

	TTF_Init();
	int pt_size = 40;
	TTF_Font* font = TTF_OpenFont("Seven Segment.ttf", pt_size);
	if (! font) {
		printf("TTF Open Font failed: %s\n", TTF_GetError());
		exit (1);
	}

	// TTF_SetFontOutline (font, 2);

	SDL_Color text_color = {
		0xF0, 0, 0, 0xFF
	};

	SDL_Surface* font_surf;
	SDL_Texture* font_texture;

	window = SDL_CreateWindow (
			"Advent of Code 2019-12-13",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			38 * 10,
			21 * 10,
			SDL_WINDOW_OPENGL
	);

	if (window == NULL) {
		printf("Failed to init window: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer (window, -1, 0);

	SDL_SetRenderDrawBlendMode (renderer, SDL_BLENDMODE_BLEND);

	SDL_Event ev;
	SDL_Rect rect = { .x = 0, .y = 0, .w = 10, .h = 10 };
	while (1) {

		/* Draw pixels */

		for (int y = 0; y < 21; y++) {
			for (int x = 0; x < 38; x++) {
				uint8_t r = 0, g = 0, b = 0, a = 0xFF;
				rect.x = x * 10;
				rect.y = y * 10;
				switch (p->screen[y][x]) {
					case empty_tile: break;
					case wall_tile: r = 0xCB; g = 0x41; b = 0x54; break;
					case block_tile: b = 0xFF; break;
					case horizontal_paddle_tile: g = 0xFF; break;
					case ball_tile: r = 0xFF; g = 0xFF; b = 0xFF; break;
					default: r = 0xFF;
				}
				SDL_SetRenderDrawColor (renderer, r, g, b, a);
				SDL_RenderFillRect (renderer, &rect);
			}
		}

		/* Draw pixel borders */

		SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0xFF);

		for (int y = 1; y < 21; y++) {
			SDL_RenderDrawLine (renderer, 0, 10 * y, 10 * 38, 10 * y);
		}

		for (int x = 1; x < 38; x++) {
			SDL_RenderDrawLine (renderer, 10 * x, 0, 10 * x, 10 * 21);
		}

		/* Draw score */

		char str[21];
		sprintf(str, "%lu", p->score);

		font_surf = TTF_RenderText_Solid (font, str, text_color);

		SDL_Rect rect = { 10, 5, font_surf->w, font_surf->h };

		SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0xFF / 2);
		SDL_RenderFillRect (renderer, &rect);

		font_texture = SDL_CreateTextureFromSurface (renderer, font_surf);
		SDL_RenderCopy (renderer, font_texture, NULL, &rect);

		SDL_RenderPresent (renderer);

		if (SDL_PollEvent (&ev)) {
			switch (ev.type) {
				case SDL_KEYDOWN:
					switch (ev.key.keysym.scancode) {
						case SDL_SCANCODE_LEFT:
							p->input = left;
							break;
						case SDL_SCANCODE_RIGHT:
							p->input = right;
							break;
						case SDL_SCANCODE_X:
							exit (0);
						default: break;
					}
					break;
				case SDL_KEYUP:
					p->input = neutral;
					break;
				case SDL_QUIT: exit (0);
			}
		}
	}
}

void make_gpu_port (struct gpu_port* mem) {
	mem->proc.put = gpu_port_put;
	mem->proc.get = gpu_port_get;
	mem->x = 0;
	mem->y = 0;
	mem->next = next_x;

	pthread_t thread;
	pthread_create(&thread, NULL, init_sdl, mem);
}
