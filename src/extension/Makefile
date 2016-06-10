CC      = gcc
CFLAGS  = `sdl-config --libs --cflags` -std=c99 -lm -Wall -pedantic
LIBS = -lSDL_image -lSDL_mixer -lwiringPi
PIFLAG = #Intentionally blank


.SUFFIXES: .c .o

.PHONY: all clean pi

all: FlappyLeague

FlappyLeague: GameEdit.o Control.o TimerObj.o UtilObj.o Debug.o SquareObj.o CircleObj.o linkedList.o Main.o Menu.o collisions.o
	$(CC) $^ $(CFLAGS) $(PIFLAG) -o FlappyLeague $(LIBS)

%.o : %.c %.h
	$(CC) $(CFLAGS) $(patsubst %.o, %.c, $@) -c -o $@ $(LIBS)

pi:
	make all PIFLAG="-DRPI"

clean:
	rm -f $(wildcard *.o)
	rm -f FlappyLeague

#Use this if you want to compile it manually
#gcc UtilObj.c Debug.c CircleObj.c linkedList.c Main.c `sdl-config --libs --cflags` -std=c99 -o daGame -lSDL_image -lSDL_mixer
