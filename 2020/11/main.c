#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH  96
#define HEIGHT 90

char grids[2][HEIGHT][WIDTH];

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

int main() {
	memset(grids, 0, HEIGHT * WIDTH * 2);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			grids[0][y][x] = getchar();
			grids[1][y][x] = grids[0][y][x];
		}
		// newline
		getchar();
	}


	int current = 0;
	int other = 1;

	for (int loops = 0; ; loops++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				int adj_count = 0;

				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i == 0 && j == 0) continue;
						/* [j, i] direction vector */
						int cx = x, cy = y;
						do {
							cx += j;
							cy += i;
							if (cx < 0 || cy < 0 || cx >= WIDTH || cy >= HEIGHT) break;;
							char cell = grids[current][cy][cx];
							if (cell == '#') {
								++adj_count;
								break;
							} else if (cell == 'L') {
								break;
							}
						} while (1);
					}
				}

				/*
				for (int i = max(0, y - 1); i <= min(HEIGHT - 1, y + 1); i++) {
					for (int j = max(0, x - 1); j <= min(WIDTH - 1, x + 1); j++) {
						if (i == y && j == x) continue;
						if (grids[current][i][j] == '#') {
							adj_count += 1;
						}
					}
				}
				*/

				// printf("[%d, %d] = %d\n", x, y, adj_count);

				switch (grids[current][y][x]) {
					case '.': break;
					case 'L': {
								  if (adj_count == 0) {
									  grids[other][y][x] = '#';
								  } else {
									  grids[other][y][x] = 'L';
								  }
								  break;
							  }
					case '#': {
								  if (adj_count >= 5) {
									  grids[other][y][x] = 'L';
								  } else {
									  grids[other][y][x] = '#';
								  }
								  break;
							  }
				}
			}
		}

		int all_same = 1;
		int occ_count = 0;
		// printf("\x1b[H\x1b[J");
		printf("loop %d, current = %d, other = %d\n", loops, current, other);
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				printf("%c",grids[current][y][x]);
				all_same &= (grids[current][y][x] == grids[other][y][x]);
				occ_count += (grids[current][y][x] == '#');
			}
			printf("\n");
		}

		if (all_same) {
			printf("%d occupied\n", occ_count);
			break;
		}

		// if (loops > 5) break;

		current ^= other;
		other   ^= current;
		current ^= other;
	}

	/*
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (grids[0][y][x] == '\n') {
				printf("N");
			} else {
				printf("%c", grids[0][y][x]);
			}
		}
		printf("\n");
	}
	*/

	return 0;
}
