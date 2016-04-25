CFLAGS=-Wall -fpic -coverage -lm -std=c11
CC=gcc

all: playdom player utests

playdom: ../playdom.c dominion.o
	$(CC) -o playdom ../playdom.c -g dominion.o cards.o rngs.o $(CFLAGS)
	$(MAKE) clean_obj

player: ../player.c dominion.o interface.o
	$(CC) -o player ../player.c -g dominion.o cards.o rngs.o interface.o $(CFLAGS)
	$(MAKE) clean_obj

utests: unittest1.o unittest2.o unittest3.o unittest4.o dominion.o
	$(CC) -o utest1 unittest1.o dominion.o cards.o rngs.o $(CFLAGS)
	$(CC) -o utest2 unittest1.o dominion.o cards.o rngs.o $(CFLAGS)
	$(CC) -o utest3 unittest1.o dominion.o cards.o rngs.o $(CFLAGS)
	$(CC) -o utest4 unittest1.o dominion.o cards.o rngs.o $(CFLAGS)
	$(MAKE) clean_obj
	
# unit tests
unittest1.o: ../unit_tests/unittest1.c
	$(CC) -c ../unit_tests/unittest1.c $(CFLAGS)

unittest2.o: ../unit_tests/unittest2.c
	$(CC) -c ../unit_tests/unittest2.c $(CFLAGS)
	
unittest3.o: ../unit_tests/unittest3.c
	$(CC) -c ../unit_tests/unittest3.c $(CFLAGS)
	
unittest4.o: ../unit_tests/unittest4.c
	$(CC) -c ../unit_tests/unittest4.c $(CFLAGS)

	
rngs.o: ../rngs.h ../rngs.c
	$(CC) -c ../rngs.c -g  $(CFLAGS) -Wno-unused-but-set-variable

cards.o: ../cards.h ../cards.c
	$(CC) -c ../cards.c -g $(CFLAGS)
	
dominion.o: ../dominion.h ../dominion.c rngs.o cards.o
	$(CC) -c ../dominion.c -g -D DEBUG $(CFLAGS)

interface.o: ../interface.h ../interface.c
	$(CC) -c ../interface.c -g  $(CFLAGS)

# cleaners
clean_obj:
	rm -f *.o *.so
	
clean: clean_obj
	rm -f playdom.exe playdom test.exe test player utest1 utest2 utest3 utest4 cardtest1 cardtest2 cardtest3 cardtest4 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.a *.dSYM
