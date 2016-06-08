CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99 `sdl-config --libs --cflags`
#SDL_CFLAGS := $(shell sdl-config --cflags) -std=c99
#SDL_LDFLAGS := $(shell sdl-config --libs)
#$(SDL_CFLAGS) $(SDL_LDFLAGS)
LIBS = -lSDL_image


.SUFFIXES: .c .o

.PHONY: all clean

all: daGame

daGame: UtilObj.o Debug.o CircleObj.o linkedList.o Tminus.o
	$(CC) UtilObj.o Debug.o CircleObj.o linkedList.o Tminus.o $(CFLAGS) -o daGame $(LIBS)

CircleObj.o : CircleObj.c CircleObj.h
	$(CC)  CircleObj.c $(CFLAGS) -o CircleObj.o $(LIBS)

linkedList.o : linkedList.c linkedList.h
	$(CC)  linkedList.c $(CFLAGS) -o linkedList.o $(LIBS)

Tminus.o : Tminus.c Tminus.h
	$(CC)  Tminus.c $(CFLAGS) -o Tminus.o $(LIBS)

Debug.o : Debug.c Debug.h
	$(CC)  Debug.c $(CFLAGS) -o Debug.o $(LIBS)

UtilObj.o : UtilObj.c UtilObj.h
	$(CC)  UtilObj.c $(CFLAGS) -o UtilObj.o $(LIBS)

clean:
	rm -f $(wildcard *.o)
	rm -f daGame

#Use this if you want to compile it manually
#gcc UtilObj.c Debug.c CircleObj.c linkedList.c Tminus.c `sdl-config --libs --cflags` -std=c99 -o daGame -lSDL_image
