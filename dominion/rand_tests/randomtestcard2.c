#include <stdio.h>
#include <stdlib.h>
#include "../test_helper.h"
#include "../dominion.h"

// test great_hall
int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
  	gameState* state = gsGenRand(seed, great_hall);
  	gameState* initial = gsCopy(state);
  	
  	playCard(0,0,0,0,state);
  	
  	
    return 0;
}