CC      = gcc
#CFLAGS  = `sdl-config --libs --cflags`
SDL_CFLAGS := $(shell sdl-config --cflags) -std=c99
SDL_LDFLAGS := $(shell sdl-config --libs)
LIBS = -lSDL_image


.SUFFIXES: .c .o

.PHONY: all clean

all: daGame

daGame: UtilObj.o CircleObj.o linkedList.o Tminus.o Debug.o
	$(CC) UtilObj.o CircleObj.o linkedList.o Tminus.o Debug.o $(SDL_CFLAGS) $(SDL_LDFLAGS) -o daGame $(LIBS)

CircleObj.o : CircleObj.c CircleObj.h
	$(CC)  CircleObj.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -o CircleObj.o $(LIBS)

linkedList.o : linkedList.c linkedList.h
	$(CC)  linkedList.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -o linkedList.o $(LIBS)

Tminus.o : Tminus.c Tminus.h
	$(CC)  Tminus.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -o Tminus.o $(LIBS)

Debug.o : Debug.c Debug.h
	$(CC)  Debug.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -o Debug.o $(LIBS)

UtilObj.o : UtilObj.c UtilObj.h
	$(CC)  UtilObj.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -o UtilObj.o $(LIBS)

clean:
	rm -f $(wildcard *.o)
	rm -f daGame
