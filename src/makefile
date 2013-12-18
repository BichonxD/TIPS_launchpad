
CC = gcc

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
H = $(wildcard *.h)
H_INC = ../lib/inc/fmod.h ../lib/inc/fmod_errors.h

CFLAGS = -Wall -g -std=c99 -D_GNU_SOURCE
LIB = `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf  ../lib/lib/libfmodex-4.44.23.so

PROG = KeyLaunchpad

all : .c.o 
	@$(CC) -o $(PROG) $(OBJS) $(LIB)
	
profil : .c.o
	@$(CC) -o $(PROG) $(OBJS) $(LIB) -pg

.c.o : 
	@$(CC) $(CFLAGS) $(LIB) $(H) $(H_INC) -c $(SRC)

clean :
	@-rm *.o *.gch ../lib/inc/*.gch ../lib/lib/*.gch *~ $(PROG)

total : clean all
