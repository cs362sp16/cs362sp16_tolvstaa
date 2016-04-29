CFLAGS=-Wall -fpic -coverage -lm -std=c11
CC=gcc

all: playdom player utests ctests

playdom: ../playdom.c dominion.o
	$(CC) -o playdom ../playdom.c -g dominion.o cards.o rngs.o $(CFLAGS)

player: ../player.c dominion.o interface.o
	$(CC) -o player ../player.c -g dominion.o cards.o rngs.o interface.o $(CFLAGS)
	
autotest: utests ctests
	../uctestrunner.sh > ../unittestresults.out

TDEPS=dominion.o cards.o rngs.o test_helper.o
utests: unittest1.o unittest2.o unittest3.o unittest4.o $(TDEPS)
	$(CC) -o unittest1 unittest1.o $(TDEPS) $(CFLAGS)
	$(CC) -o unittest2 unittest2.o $(TDEPS) $(CFLAGS)
	$(CC) -o unittest3 unittest3.o $(TDEPS) $(CFLAGS)
	$(CC) -o unittest4 unittest4.o $(TDEPS) $(CFLAGS)

ctests: cardtest1.o cardtest2.o cardtest3.o cardtest4.o $(TDEPS)
	$(CC) -o cardtest1 cardtest1.o $(TDEPS) $(CFLAGS)
	$(CC) -o cardtest2 cardtest2.o $(TDEPS) $(CFLAGS)
	$(CC) -o cardtest3 cardtest3.o $(TDEPS) $(CFLAGS)
	$(CC) -o cardtest4 cardtest4.o $(TDEPS) $(CFLAGS)
	
# unit tests
unittest1.o: ../unit_tests/unittest1.c
	$(CC) -c ../unit_tests/unittest1.c $(CFLAGS)

unittest2.o: ../unit_tests/unittest2.c
	$(CC) -c ../unit_tests/unittest2.c $(CFLAGS)
	
unittest3.o: ../unit_tests/unittest3.c
	$(CC) -c ../unit_tests/unittest3.c $(CFLAGS)
	
unittest4.o: ../unit_tests/unittest4.c
	$(CC) -c ../unit_tests/unittest4.c $(CFLAGS)

# card tests
cardtest1.o: ../card_tests/cardtest1.c
	$(CC) -c ../card_tests/cardtest1.c $(CFLAGS)

cardtest2.o: ../card_tests/cardtest2.c
	$(CC) -c ../card_tests/cardtest2.c $(CFLAGS)

cardtest3.o: ../card_tests/cardtest3.c
	$(CC) -c ../card_tests/cardtest3.c $(CFLAGS)

cardtest4.o: ../card_tests/cardtest4.c
	$(CC) -c ../card_tests/cardtest4.c $(CFLAGS)

# test helpers
test_helper.o: ../test_helper.c ../test_helper.h
	$(CC) -c ../test_helper.c $(CFLAGS)
	
	
rngs.o: ../rngs.h ../rngs.c
	$(CC) -c ../rngs.c -g  $(CFLAGS) -Wno-unused-but-set-variable

cards.o: ../cards.h ../cards.c
	$(CC) -c ../cards.c -g $(CFLAGS) -D DEBUG
	
dominion.o: ../dominion.h ../dominion.c rngs.o cards.o
	$(CC) -c ../dominion.c -g -D DEBUG $(CFLAGS)

interface.o: ../interface.h ../interface.c
	$(CC) -c ../interface.c -g  $(CFLAGS)

# cleaners
clean_obj:
	rm -f *.o *.so 
	
clean: clean_obj clean_gcov
	rm -f playdom.exe playdom test.exe test player unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.a *.dSYM
