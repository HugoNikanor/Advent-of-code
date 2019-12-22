#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "fs.h"

int main(int argc, char* argv[argc]) {
	if (argc == 1) exit (1);

	truncate(argv[1], sizeof(struct filesystem));
}
