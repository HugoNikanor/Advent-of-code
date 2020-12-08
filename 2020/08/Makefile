.PHONY: all clean

CFLAGS := -Wall -pedantic -std=c99

all: asm

include deps.mk

lex.yy.c: asm.l
	$(LEX) $<

LDFLAGS := -lfl


asm: asm.o eval.o types.o lex.yy.o 
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	-rm *.o
