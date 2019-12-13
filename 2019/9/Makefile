.PHONE: all
CFLAGS=-Wall -Wextra -pedantic -ggdb
CFILES = $(wildcard *.c)
OFILES = $(CFILES:*.c=obj/*.o)

all: main

main: $(OFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<
