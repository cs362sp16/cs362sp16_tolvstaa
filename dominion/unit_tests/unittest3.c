#include <stdio.h>
#include <stdlib.h>
#include "../dominion.h"
#include "../test_helper.h"

int main() {
    printf("Testing fullDeckCount...\n");
    gameState* state = gsGenPlain();
    
    state->deckCount[0] = 3;
    state->deck[0][0] = smithy;
    state->deck[0][1] = duchy;
    state->deck[0][2] = village;
    
    state->handCount[0] = 2;
    state->hand[0][0] = village;
    state->hand[0][1] = smithy;
    
    state->discardCount[0] = 5;
    state->discard[0][0] = smithy;
    state->discard[0][1] = smithy;
    state->discard[0][2] = gardens;
    state->discard[0][3] = sea_hag;
    state->discard[0][4] = smithy;
    
    my_assert(fullDeckCount(0, smithy, state) == 5);
    
    free(state);
    printf("fullDeckCount testing complete.\n");
    return 0;
}