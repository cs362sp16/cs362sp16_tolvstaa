#include "../dominion.h"
#include "../test_helper.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Testing smithy card...\n");
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
    
    state->numActions = 1;
    
    playCard(1, 0, 0, 0, state);
    my_assert(state->handCount[0] == 5);
    
    free(state);
    printf("smithy card testing complete.\n");
	return 0;
}
