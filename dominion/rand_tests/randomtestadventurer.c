#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "../test_helper.h"
#include "../dominion.h"

void no_halt(int signal) {
    my_assert(0);
}

int treasureCount(gameState* state) {
    int count = 0;
    int card;
 	for(int i=0; i<state->handCount[0]; i++) {
 	    card = state->hand[0][i];
 	    if(card == copper || card == silver || card == gold) {
 	        count++;
 	    }
 	}
 	return count;
}

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    
 	gameState* state = gsGenRand(seed, adventurer);
 	gameState* initial = gsCopy(state);
 	
 	signal(SIGALRM, no_halt);
 	
 	alarm(5);
 	playCard(0, 0, 0, 0, state);
 	alarm(0);
    
 	my_assert(treasureCount(state) == treasureCount(initial) + 2); // has 2 more treasure cards in hand
 	my_assert(initial->deckCount[0] - state->deckCount[0] > 1); // drew at least 2 cards
 	
 	printf("Testing complete.\n");
    return 0;
}