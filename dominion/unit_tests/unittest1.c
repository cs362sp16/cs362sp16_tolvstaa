#include <stdio.h>
#include <stdlib.h>
#include "../dominion.h"
#include "../test_helper.h"

int main() {
    printf("Testing nextPlayer...\n");
    gameState* state = gsGenPlain();
    
    state->numPlayers = 3;
    state->whoseTurn = 1;
    my_assert(nextPlayer(state) == 2);
    
    state->whoseTurn = 2; 
    my_assert(nextPlayer(state) == 0);
    
    free(state);
    printf("nextPlayer testing complete.\n");
    return 0;
}