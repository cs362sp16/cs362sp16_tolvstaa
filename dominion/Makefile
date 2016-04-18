CFLAGS=-Wall -fpic -coverage -lm -std=c11
CC=gcc

all: playdom player

playdom: playdom.c dominion.o
	$(CC) -o playdom playdom.c -g dominion.o cards.o rngs.o $(CFLAGS)

player: player.c dominion.o interface.o
	$(CC) -o player player.c -g dominion.o cards.o rngs.o interface.o $(CFLAGS)

rngs.o: rngs.h rngs.c
	$(CC) -c rngs.c -g  $(CFLAGS) -Wno-unused-but-set-variable

cards.o: cards.h cards.c
	$(CC) -c cards.c -g $(CFLAGS)
	
dominion.o: dominion.h dominion.c rngs.o cards.o
	$(CC) -c dominion.c -g -D DEBUG $(CFLAGS)

interface.o: interface.h interface.c
	$(CC) -c interface.c -g  $(CFLAGS)

clean:
	rm -f *.o playdom.exe playdom test.exe test player unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.a *.dSYM
