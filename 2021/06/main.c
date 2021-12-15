#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 10_747_312

int main (int argc, char** argv) {
	long long int population[9] = { 0 }, next_population[9] = { 0 };
	char* line = NULL;
	size_t len;
	if (getline(&line, &len, stdin) == -1) {
		free(line);
		printf("Read failed\n");
		exit (1);
	}

	/* replace trailing newline */
	line[len - 2] = ',';

	char* ptr = strtok(line, ",");
	++population[atoi(ptr)];
	while ((ptr = strtok(NULL, ",")) != NULL) {
		++population[atoi(ptr)];
	}

	printf("{ %lli", population[0]);
	for (int i = 1; i < 9; i++) {
		printf(", %lli", population[i]);
	}
	puts(" }");

	int days;
	if (argc >= 2) {
		days = atoi(argv[1]);
	} else {
		days = 18;
	}

	// int *pop = &population[0], *next_pop = &next_population[0], *swap_ptr;
	for (int day = 0; day < days; day++) {
		printf("%i { %lli", day, population[0]);
		for (int i = 1; i < 9; i++) {
			printf(", %lli", population[i]);
		}
		puts(" }");

		memset(next_population, 0, sizeof(next_population));
		for (int i = 0; i < 8; i++) {
			next_population[i] = population[i + 1];
		}
		next_population[8] += population[0];
		next_population[6] += population[0];

		memcpy(population, next_population, sizeof(next_population));
		memset(next_population, 0, sizeof(next_population));
		// swap_ptr = pop;
		// pop = next_pop;
		// next_pop = swap_ptr;

	}

	long long sum = 0;
	long long next_sum = 0;
	for (int i = 0; i < 9; i++) {
		sum += population[i];
		next_sum += next_population[i];
	}
	printf("sum = %lli, next_sum = %lli\n", sum, next_sum);

	free (line);
}
