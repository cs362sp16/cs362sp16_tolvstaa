#include <stdio.h>
#include <stdlib.h>
#include "../dominion.h"
#include "../test_helper.h"

int discardCard(int, int, gameState*, int);

int main() {
    printf("Testing discardCard...\n");
    gameState* state = gsGenPlain();
    
    state->hand[0][0] = village;
    state->hand[0][1] = duchy;
    state->hand[0][2] = gardens;
    state->hand[0][3] = smithy;
    
    state->handCount[0] = 4;
    
    discardCard(1, 0, state, 0);
    
    my_assert(state->handCount[0] == 3);
    my_assert(state->hand[0][0] == village);
    my_assert(state->hand[0][1] == smithy);
    my_assert(state->hand[0][2] == gardens);
    
    free(state);
    printf("discardCard testing complete.\n");
    return 0;
}