CFLAGS=-Wall -fpic -coverage -lm -std=c11
CC=gcc

UTEST=unittest1 unittest2 unittest3 unittest4
CTEST=cardtest1 cardtest2 cardtest3 cardtest4
RTEST=randomtestcard1 randomtestcard2 randomtestadventurer
TDEPS=dominion.o cards.o rngs.o test_helper.o

all: playdom player test

tarantula: ttest
	../tarantula/tarantula.py

ttest: ../tarantula/ttestdriver.c $(TDEPS)
	$(CC) -o ttest ../tarantula/ttestdriver.c $(TDEPS) $(CFLAGS)
	
difftest:
	../diffdominion ../ ~/cs362sp16_phillels/dominion 42

playdom: ../playdom.c dominion.o
	$(CC) -o playdom ../playdom.c -g dominion.o cards.o rngs.o $(CFLAGS)

player: ../player.c dominion.o interface.o
	$(CC) -o player ../player.c -g dominion.o cards.o rngs.o interface.o $(CFLAGS)

testdominion: ../testdominion.c dominion.o interface.o
	$(CC) -o testdominion ../testdominion.c dominion.o cards.o rngs.o interface.o $(CFLAGS)
	./testdominion 42
	
test: $(UTEST) $(CTEST) $(RTEST)

autotest: test
	../uctestrunner.sh
	../rtestrunner.sh


# tests
$(UTEST): %: ../unit_tests/%.c $(TDEPS)
	$(CC) -o $@ $< $(TDEPS) $(CFLAGS)
	
$(CTEST): %: ../card_tests/%.c $(TDEPS)
	$(CC) -o $@ $< $(TDEPS) $(CFLAGS)

$(RTEST): %: ../rand_tests/%.c $(TDEPS)
	$(CC) -o $@ $< $(TDEPS) $(CFLAGS)

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


clean:
	rm -f playdom test player testdominion
	rm -f $(RTEST) $(CTEST) $(UTEST)
	rm -f *.o *.so
	rm -f *.gcov *.gcda *.gcno *.a *.dSYM
