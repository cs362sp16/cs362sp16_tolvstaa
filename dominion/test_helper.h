#ifndef _TEST_HELPER
#define _TEST_HELPER

#include <stdio.h>
#include "dominion.h"

#ifdef ASSERT_TRACKER
    #define my_assert(X) { \
        if(!(X)) { \
            fprintf(stderr, "Assertion failed at %s:%d in function %s:\n\tCondition: %s\n", \
                __FILE__, __LINE__, __FUNCTION__, (#X)); \
                assert_track += 1; \
        } \
    }
#else
    #define my_assert(X) { \
        if(!(X)) { \
            fprintf(stderr, "Assertion failed at %s:%d in function %s:\n\tCondition: %s\n", \
                __FILE__, __LINE__, __FUNCTION__, (#X)); \
        } \
    }
#endif

int randCard(int);
gameState* gsGenPlain();
gameState* gsGenRand(int, int);
gameState* gsCopy(const gameState*);

#endif