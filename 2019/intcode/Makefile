.PHONE: all
CFLAGS=-Wall -Wextra -pedantic -ggdb $(shell pkg-config --cflags sdl2)
LDFLAGS=$(shell pkg-config --libs sdl2 SDL2_ttf)
CFILES = $(wildcard *.c)
OFILES = $(CFILES:%.c=obj/%.o)

all: main

main: $(OFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm obj/*
