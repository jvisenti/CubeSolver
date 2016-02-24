PLATFORM = $(shell uname)


## Compilation flags
##comment out one or the other 
##debugging 
#CFLAGS = -g
##release
CFLAGS = -O1 -DNDEBUG

CFLAGS+= -Wall

CC = gcc $(CFLAGS)

PROGS = BHCubeSolver

default: $(PROGS)

BHCubeSolver: BHCubeSolver.o BHCSPiece.o BHCSWorld.o BHDancingLinks.o RVStack.o
	$(CC) -o $@ BHCubeSolver.o BHCSPiece.o BHCSWorld.o BHDancingLinks.o RVStack.o

BHCubeSolver.o: BHCubeSolver.c BHCSWorld.h
	$(CC) -c BHCubeSolver.c  -o $@
    
BHCSWorld.o: BHCSWorld.c BHCSWorld.h BHCSPiece.h BHDancingLinks.h RVStack.h
	$(CC) -c BHCSWorld.c -o $@

clean:	
	rm *.o
	rm BHCubeSolver


