CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate

#assemble: assemble.o

emulate: emulate.o helperFunctions.o emulate_Branch.o emulate_DP.o\
	 emulate_Error.o emulate_Mul.o emulate_SDT.o
	$(CC) $(CFLAGS) emulate.o helperFunctions.o emulate_Branch.o emulate_DP.o\
	 emulate_Error.o emulate_Mul.o emulate_SDT.o -o emulate

emulate.o : emulate.c emulate.h
	$(CC) $(CFLAGS) emulate.c -c -o emulate.o

helperFunctions.o : helperFunctions.c helperFunctions.h
	$(CC) $(CFLAGS) helperFunctions.c -c -o helperFunctions.o

emulate_Branch.o : emulate_Branch.c emulate_Branch.h
	$(CC) $(CFLAGS) emulate_Branch.c -c -o emulate_Branch.o

emulate_DP.o : emulate_DP.c emulate_DP.h
	$(CC) $(CFLAGS) emulate_DP.c -c -o emulate_DP.o

emulate_Error.o : emulate_Error.c emulate_Error.h
	$(CC) $(CFLAGS) emulate_Error.c -c -o emulate_Error.o

emulate_Mul.o : emulate_Mul.c emulate_Mul.h
	$(CC) $(CFLAGS) emulate_Mul.c -c -o emulate_Mul.o

emulate_SDT.o : emulate_SDT.c emulate_SDT.h
	$(CC) $(CFLAGS) emulate_SDT.c -c -o emulate_SDT.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
