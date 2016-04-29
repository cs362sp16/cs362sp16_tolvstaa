#include "../dominion.h"
#include "../test_helper.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Testing village card...\n");
    gameState* state = gsGenPlain();
    
    state->deckCount[0] = 5;
    state->deck[0][0] = province;
    state->deck[0][1] = gold;
    state->deck[0][2] = copper;
    state->deck[0][3] = smithy;
    state->deck[0][4] = duchy;
    
    state->handCount[0] = 3;
    state->hand[0][0] = gardens;
    state->hand[0][1] = smithy;
    state->hand[0][2] = village;
    
    state->numActions = 2;
    
    playCard(2, 0, 0, 0, state);
    my_assert(state->handCount[0] == 3);
    my_assert(state->numActions == 3);
    
    free(state);
    printf("village card testing complete.\n");
	return 0;
}
