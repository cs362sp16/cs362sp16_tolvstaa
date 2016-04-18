#ifndef _CARDS
#define _CARDS

#include "dominion.h"
#include "dominion_helpers.h"

int adventurer_effect       (gameState*);
int council_room_effect     (gameState*, int);
int feast_effect            (gameState*, int);
int gardens_effect          ();
int mine_effect             (gameState*, int, int, int);
int remodel_effect          (gameState*, int, int, int);
int smithy_effect           (gameState*, int);
int village_effect          (gameState*, int);
int baron_effect            (gameState*, int);
int great_hall_effect       (gameState*, int);
int minion_effect           (gameState*, int, int, int);
int steward_effect          (gameState*, int, int, int, int);
int tribute_effect          (gameState*);
int ambassador_effect       (gameState*, int, int, int);
int cutpurse_effect         (gameState*, int);
int embargo_effect          (gameState*, int, int);
int outpost_effect          (gameState*, int);
int salvager_effect         (gameState*, int, int);
int sea_hag_effect          (gameState*);
int treasure_map_effect     (gameState*, int);
    
#endif