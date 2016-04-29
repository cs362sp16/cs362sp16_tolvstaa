#include <stdio.h>
#include <stdlib.h>
#include "../dominion.h"
#include "../test_helper.h"

int main() {
    printf("Testing isGameOver...\n");
    gameState* initial = gsGenPlain();
    
    for(int i=0; i<27; i++)
        initial->supplyCount[i] = 5;
    
    my_assert(!isGameOver(initial));
    
    gameState* state = gsCopy(initial);
    state->supplyCount[sea_hag] = 0;
    my_assert(!isGameOver(state));
    state->supplyCount[treasure_map] = 0;
    my_assert(!isGameOver(state));
    state->supplyCount[salvager] = 0;
    my_assert(isGameOver(state));
    free(state);
    
    state = gsCopy(initial);
    state->supplyCount[province] = 0;
    my_assert(isGameOver(state));
    free(state);
    
    free(initial);
    
    printf("isGameOver testing complete.\n");
    return 0;
}