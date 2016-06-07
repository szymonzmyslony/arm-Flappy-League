CC      = gcc
CFLAGS  = `sdl-config --libs --cflags`
LIBS = -lSDL_image


.SUFFIXES: .c .o

.PHONY: all clean

all: daGame

daGame: CircleObj.o linkedList.o Tminus.o
	$(CC)  CircleObj.o linkedList.o Tminus.o $(CFLAGS) -o daGame $(LIBS)

CircleObj.o : CircleObj.c CircleObj.h
	$(CC)  CircleObj.c $(CFLAGS) -o CircleObj.o $(LIBS)

linkedList.o : linkedList.c linkedList.h
	$(CC)  linkedList.c $(CFLAGS) -o linkedList.o $(LIBS)

Tminus.o : Tminus.c Tminus.h
	$(CC)  Tminus.c $(CFLAGS) -o Tminus.o $(LIBS)

clean:
	rm -f $(wildcard *.o)
	rm -f daGame
