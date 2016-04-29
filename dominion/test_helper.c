#include "test_helper.h"
#include "dominion.h"
#include <stdlib.h>

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

gameState* gsGenRand() {
    gameState* gs = new_gs();
    
    // give a sane randomization of gameState
    
    return gs;
}

gameState* gsCopy(const gameState* s1) {
    gameState* s2 = new_gs();
    return (gameState*) memorycopy(s2, s1, sizeof(gameState));
}