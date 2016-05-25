#include "dominion.h"
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>

int hasAction(int player, gameState* state) {
    for(int i=0; i<state->handCount[player]; i++)
        if(state->hand[player][i] >= adventurer)
            return 1;
    return 0;
}

int main(int argc, char** argv) {
    int seed;
    if(argc>1) {
        seed = atoi(argv[1]);
    } else {
        fprintf(stderr, "Please specify a seed.\n");
        return -1;
    }
    srand(seed);
    gameState* state = calloc(1, sizeof(gameState));
    state->numPlayers = (rand() % 2) + 2;
    
    // set kingdom cards
    int k[10] = {0};
    for(int i=0; i<10; i++) {
        int duplicate;
        do {
            k[i] = (rand() % treasure_map-adventurer) + adventurer;
            duplicate = 0;
            for(int j=0; j<i; j++) if(k[j] == k[i]) duplicate = 1;
        } while(duplicate);
    }
    initializeGame(state->numPlayers, k, seed, state);
    int fullCopper = fullDeckCount(0,copper,state);
    int turns = 0;
    // main game loop
    do {
        turns++;
        printf("Turn %d.\n", turns);
        if(hasAction(state->whoseTurn, state)) {
            int card;
            do {
                card = rand() % state->handCount[state->whoseTurn];
            } while(state->hand[state->whoseTurn][card] >= adventurer);
            
            playCard(card, rand()%3, rand()%3, rand()%3, state);
        }
        int buy;
        do {
            buy = (rand() % treasure_map-adventurer) + adventurer;
        } while(countHandCoins(state->whoseTurn, state) > 1 &&
            (!supplyCount(buy, state) ||
            getCardCost(buy) >= countHandCoins(state->whoseTurn, state)));
        
        buyCard(buy, state);
        endTurn(state);
        //printState(state);
    } while(!isGameOver(state));
    
    int players[MAX_PLAYERS];
    getWinners(players, state);
    int postCopper = fullDeckCount(0,copper,state);
    
    printf("Copper diff: %d\n", postCopper-fullCopper);
    printState(state);
    free(state);
    return 0;
}