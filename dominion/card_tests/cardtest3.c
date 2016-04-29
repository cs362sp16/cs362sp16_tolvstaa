#include "../dominion.h"
#include "../test_helper.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Testing steward card...\n");
    gameState* initial = gsGenPlain(), * state;
    
    initial->deckCount[0] = 5;
    initial->deck[0][0] = province;
    initial->deck[0][1] = gold;
    initial->deck[0][2] = copper;
    initial->deck[0][3] = smithy;
    initial->deck[0][4] = duchy;
    
    initial->handCount[0] = 3;
    initial->hand[0][0] = gardens;
    initial->hand[0][1] = steward;
    initial->hand[0][2] = village;

    initial->numActions = 1;
    
    state = gsCopy(initial);
    playCard(1, 1, 0, 0, state);
    my_assert(state->handCount[0] == 4);
    free(state);
    
    state = gsCopy(initial);
    playCard(1, 2, 1, 0, state);
    my_assert(state->coins == 2);
    free(state);
    
    state = gsCopy(initial);
    playCard(1, 3, 0, 1, state);
    my_assert(state->handCount[0] == 0);
    free(state);
    
    free(initial);
    printf("steward card testing complete.\n");
	return 0;
}
