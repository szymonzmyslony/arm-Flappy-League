CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: helperFunctions emulate 

#assemble: assemble.o

emulate: emulate.o helperFunctions.o
	$(CC) $(CFLAGS) emulate.o helperFunctions.o -o emulate

emulate.o : emulate.c emulate.h
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o

helperFunctions : helperFunctions.o
	$(CC) $(CFLAGS) helperFunctions.o -o helperFunctions

helperFunctions.o : helperFunctions.c helperFunctions.h
	$(CC) $(CFLAGS) helperFunctions.c -c -o helperFunctions.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f helperFunctions
