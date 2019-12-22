#include <stdio.h>
#include <limits.h>

#if 1
#define LAYERS 100
#define WIDTH 25
#define HEIGHT 6
#else
#define LAYERS 4
#define WIDTH 2
#define HEIGHT 2
#endif

char img[LAYERS][HEIGHT][WIDTH];
int digit_count[LAYERS][10] = { 0 };

int main () {
	char c;
	char* ptr = (char*) &img;
	while ((c = getchar()) != '\n') {
		c -= '0';
		*ptr = c;
		ptr++;
	}

	int min_zero = INT_MAX;
	int layer_number = 0;

	for (int l = 0; l < LAYERS; l++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				digit_count[l][img[l][y][x]] += 1;
			}
		}
		if (digit_count[l][0] < min_zero) {
			min_zero = digit_count[l][0];
			layer_number = l;
		}
	}

	printf("layer with fewest zeros = %i\n |1| * |2| = %i\n",
			layer_number,
			digit_count[layer_number][1] * digit_count[layer_number][2]);

	FILE* f = fopen("image.pbm", "w");

	fprintf(f, "P1\n%i %i\n", WIDTH, HEIGHT);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int i;
			for (int l = 0; l < LAYERS; l++) {
				i = img[l][y][x];
				if (i == 0 || i == 1) break;
			}
			fprintf(f, "%i ", i);
		}
		fprintf(f, "\n");
	}
}

