CC      = gcc
CFLAGS  = `sdl-config --libs --cflags` -std=c99 -lm -Wall -pedantic
LIBS = -lSDL_image -lSDL_mixer -lwiringPi


.SUFFIXES: .c .o

.PHONY: all clean

all: daGame

daGame: GameEdit.o Control.o UtilObj.o Debug.o SquareObj.o CircleObj.o linkedList.o Tminus.o collisions.o
	$(CC) GameEdit.o Control.o UtilObj.o Debug.o SquareObj.o CircleObj.o linkedList.o Tminus.o collisions.o $(CFLAGS) -o daGame $(LIBS)

GameEdit.o : GameEdit.c GameEdit.h
	$(CC) $(CFLAGS) GameEdit.c -c -o GameEdit.o $(LIBS)

Control.o : Control.c Control.h
	$(CC) $(CFLAGS) Control.c -c -o Control.o $(LIBS)

SquareObj.o : SquareObj.c SquareObj.h
	$(CC) $(CFLAGS) SquareObj.c -c -o SquareObj.o $(LIBS)

CircleObj.o : CircleObj.c CircleObj.h
	$(CC) $(CFLAGS) CircleObj.c -c -o CircleObj.o $(LIBS)

linkedList.o : linkedList.c linkedList.h
	$(CC) $(CFLAGS) linkedList.c -c -o linkedList.o $(LIBS)

Tminus.o : Tminus.c Tminus.h
	$(CC) $(CFLAGS) Tminus.c -c -o Tminus.o $(LIBS)

Debug.o : Debug.c Debug.h
	$(CC) $(CFLAGS) Debug.c -c -o Debug.o $(LIBS)

UtilObj.o : UtilObj.c UtilObj.h
	$(CC) $(CFLAGS) UtilObj.c -c -o UtilObj.o $(LIBS)

collisions.o : collisions.c collisions.h
	$(CC) $(CFLAGS) collisions.c -c -o collisions.o $(LIBS)

clean:
	rm -f $(wildcard *.o)
	rm -f daGame

#Use this if you want to compile it manually
#gcc UtilObj.c Debug.c CircleObj.c linkedList.c Tminus.c `sdl-config --libs --cflags` -std=c99 -o daGame -lSDL_image -lSDL_mixer
