CFLAGS = -Wall -fpic -coverage -lm -std=c11

all: playdom player

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

cards.o: cards.h cards.c
	gcc -c cards.c -g $(CFLAGS)
	
dominion.o: dominion.h dominion.c rngs.o cards.o
	gcc -c dominion.c -g -D DEBUG $(CFLAGS)

playdom: playdom.c dominion.o rngs.o
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

player: player.c dominion.o rngs.o interface.o
	gcc -o player player.c -g dominion.o rngs.o interface.o $(CFLAGS)

clean:
	rm -f *.o playdom.exe playdom test.exe test player unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.a *.dSYM
