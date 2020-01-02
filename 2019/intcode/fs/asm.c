#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#include "quine.h"

int main(int argc, char* argv[argc])
{
	int fd = open(argv[1], O_CREAT|O_WRONLY);
	write (fd, prog, sizeof(prog));
	if (errno != 0) {
		printf("errno = %i\n", errno);
	}
	sync();
	close(fd);
}
