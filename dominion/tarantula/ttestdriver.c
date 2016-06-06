#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define ASSERT_TRACKER
#include "../test_helper.h"
#include "../dominion.h"

int assert_track = 0;

void no_halt(int signal) {
    my_assert(0);
    exit(assert_track);
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
 	
 	alarm(2);
 	if(!seed%2) {
 	    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, 
	        sea_hag, tribute, smithy};
 	    gameState* state = gsGenPlain();
 	    initializeGame(2, k, seed+1, state);
        my_assert(state->numPlayers == 2);
        my_assert(!isGameOver(state));
        free(state);
 	}
 	
 	if(!seed%3) {
 	    gameState* state = gsGenRand(seed, adventurer);
 	    gameState* initial = gsCopy(state);
     	signal(SIGALRM, no_halt);
     	
     	
     	playCard(0, 0, 0, 0, state);
     	
        
     	my_assert(treasureCount(state) == treasureCount(initial) + 2); // has 2 more treasure cards in hand
     	my_assert(initial->deckCount[0] - state->deckCount[0] > 1); // drew at least 2 cards
     	free(state);
     	free(initial);
 	}
 	
 	alarm(0);
    return assert_track;
}