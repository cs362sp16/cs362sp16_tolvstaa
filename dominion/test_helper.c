#include "test_helper.h"
#include "dominion.h"
#include <stdlib.h>

int randCard(int seed) {
    srand(seed);
    return rand() % (treasure_map+1);
}

static inline void* memorycopy(void* dest, const void* src, size_t n) {
    for(size_t i=0; i<n; i++)
        ((char*)dest)[i] = ((char*)src)[i];
        
    return dest;
}

static inline void* memoryset(void* s, int c, size_t n) {
    for(size_t i=0; i<n; i++)
        ((char*)s)[i] = c;
        
    return s;
}

static inline gameState* new_gs() {
    return (gameState*) malloc(sizeof(gameState));
}

gameState* gsGenPlain() {
    gameState* s = new_gs();
    return memoryset(s, 0, sizeof(gameState));
}

gameState* gsGenRand(int seed, int iCard) {
    gameState* gs = new_gs();
    // give a sane randomization of gameState
    srand(seed);
    // random number of players
    gs->numPlayers = (rand() % MAX_PLAYERS)+1;
    gs->numActions = 5;
    
    for(int i=0; i<gs->numPlayers; i++) { // for each player
        gs->deckCount[i] = rand() % MAX_DECK; // random deck size
        for(int j=0; j<gs->deckCount[i]; j++) {
            gs->deck[i][j] = randCard(seed + i); // random cards
        }
        
        int limit = rand() % MAX_HAND;
        for(int j=0; j<limit && gs->handCount[i] + gs->deckCount[i] < MAX_DECK; j++) {
            gs->hand[i][j] = randCard(seed + i + j);
            gs->handCount[i]++;
        }
        
        gs->hand[0][0] = iCard; // mandatory card
        gs->handCount[0] += !(gs->handCount[0]); // add one if hand empty
    }
    
    return gs;
}

gameState* gsCopy(const gameState* s1) {
    gameState* s2 = new_gs();
    return (gameState*) memorycopy(s2, s1, sizeof(gameState));
}
