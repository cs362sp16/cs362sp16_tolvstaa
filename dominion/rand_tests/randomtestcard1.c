#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../test_helper.h"
#include "../dominion.h"

// test gardens
int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
 	gameState* state = gsGenRand(seed, gardens);
 	gameState* initial = gsCopy(state);
 	
 	playCard(0,0,0,0,state);
 	
 	my_assert(!strncmp((char*) state, (char*) initial, sizeof(gameState)));
 	
 	printf("Testing complete.\n");
    return 0;
}