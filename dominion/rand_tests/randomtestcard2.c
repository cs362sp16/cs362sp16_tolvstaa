#include <stdio.h>
#include <stdlib.h>
#include "../test_helper.h"
#include "../dominion.h"

// test great_hall
int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
  	gameState* state = gsGenRand(seed, great_hall);
  	gameState* initial = gsCopy(state);
  	
  	my_assert(state->numActions != 0);
  	my_assert(state->handCount[0] != 0);
  	
  	playCard(0,0,0,0,state);
  	
  	my_assert(state->numActions == initial->numActions);
  	my_assert(state->handCount[0] == initial->handCount[0]);
  	
  	printf("Testing complete.\n");
    return 0;
}