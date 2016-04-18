#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include "cards.h"
#include "debugprint.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

int compare(const void* a, const void* b) {
    if (*(int*)a > *(int*)b)
        return 1;

    if (*(int*)a < *(int*)b)
        return -1;
    
    return 0;
}

gameState* newGame() {
    gameState* g = malloc(sizeof(gameState));
    return g;
}

// TODO is this function ever used?
/* int* kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7, int k8, int k9, int k10) {
    int* k = malloc(10 * sizeof(int));
    k[0] = k1;
    k[1] = k2;
    k[2] = k3;
    k[3] = k4;
    k[4] = k5;
    k[5] = k6;
    k[6] = k7;
    k[7] = k8;
    k[8] = k9;
    k[9] = k10;
    return k;
} */

int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed, gameState *state) {
    //set up random number generator
    SelectStream(1);
    PutSeed((long)randomSeed);

    //check number of players
    if (numPlayers > MAX_PLAYERS || numPlayers < 2)
        return -1;

    //set number of players
    state->numPlayers = numPlayers;

    //check selected kingdom cards are different
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (j != i && kingdomCards[j] == kingdomCards[i])
                return -1;


    //initialize supply
    ///////////////////////////////
    
    
    switch(numPlayers) {
        case 2:
            state->supplyCount[curse] = 10;
            break;

        case 3:
            state->supplyCount[curse] = 20;
            break;

        default:
            state->supplyCount[curse] = 30;
    }

    //set number of Victory cards
    if (numPlayers == 2) {
        state->supplyCount[estate] =
        state->supplyCount[duchy] =
        state->supplyCount[province] = 8;
    } else {
        state->supplyCount[estate] =
        state->supplyCount[duchy] =
        state->supplyCount[province] = 12;
    }

    //set number of Treasure cards
    state->supplyCount[copper] = 60 - (7 * numPlayers);
    state->supplyCount[silver] = 40;
    state->supplyCount[gold] = 30;

    //set number of Kingdom cards
    for (int i = adventurer; i <= treasure_map; i++)    //loop all cards
        for (int j = 0; j < 10; j++)            		//loop chosen cards
            if (kingdomCards[j] == i) {
                //check if card is a 'Victory' Kingdom card
                if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens) {
                    state->supplyCount[i] = (numPlayers == 2)?8:12; 
                } else {
                    state->supplyCount[i] = 10;
                }

                break;

            } else {  //card is not in the set choosen for the game
                state->supplyCount[i] = -1;
            }

    ////////////////////////
    //supply intilization complete

    //set player decks
    for (int i = 0; i < numPlayers; i++) {
        state->deckCount[i] = 0;
        
        for (int j = 0; j < 3; j++) {
            state->deck[i][j] = estate;
            state->deckCount[i]++;
        }

        for (int j = 3; j < 10; j++) {
            state->deck[i][j] = copper;
            state->deckCount[i]++;		
        }
    }

    //shuffle player decks
    for (int i = 0; i < numPlayers; i++)
        if (shuffle(i, state) < 0)
            return -1;

    //draw player hands
    for (int i = 0; i < numPlayers; i++) {  
        //initialize hand size to zero
        state->handCount[i] = 0;
        state->discardCount[i] = 0;
        //draw 5 cards
        // for (j = 0; j < 5; j++)
        //	{
        //	  drawCard(i, state);
        //	}
    }

    //set embargo tokens to 0 for all supply piles
    for (int i = 0; i <= treasure_map; i++)
        state->embargoTokens[i] = 0;

    //initialize first player's turn
    state->outpostPlayed = 0;
    state->phase = 0;
    state->playedCardCount = 0;
    state->whoseTurn = 0;
    state->handCount[state->whoseTurn] = 0;
    
    state->numActions = 1;
    state->numBuys = 1;

    //Moved draw cards to here, only drawing at the start of a turn
    for (int it = 0; it < 5; it++)
        drawCard(state->whoseTurn, state);

    updateCoins(state->whoseTurn, state, 0);

    return 0;
}

int shuffle(int player, gameState *state) {
    int newDeck[MAX_DECK];
    int newDeckPos = 0;

    if (state->deckCount[player] < 1)
        return -1;

    qsort ((void*)(state->deck[player]), state->deckCount[player], sizeof(int), compare); 
    /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

    int card;
    while (state->deckCount[player] > 0) {
        card = floor(Random() * state->deckCount[player]);
        newDeck[newDeckPos] = state->deck[player][card];
        newDeckPos++;

        for (int i = card; i < state->deckCount[player]-1; i++) 
            state->deck[player][i] = state->deck[player][i+1];
        
        state->deckCount[player]--;
    }

    for (int i = 0; i < newDeckPos; i++) {
        state->deck[player][i] = newDeck[i];
        state->deckCount[player]++;
    }

    return 0;
}

int playCard(int handPos, int choice1, int choice2, int choice3, gameState *state) {
    // disallow improper phase or too few actions
    if (state->phase != 0 || state->numActions < 1)
        return -1;

    //get card played
    int card = handCard(handPos, state);

    //check if selected card is an action
    if (card < adventurer || card > treasure_map)
        return -1;

    //play card
    int coin_bonus = 0; 		//tracks coins gain from actions
    if (cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus) < 0)
        return -1;

    //update actions and coins (Treasure cards may be added with card draws)
    state->numActions--;
    updateCoins(state->whoseTurn, state, coin_bonus);

    return 0;
}

int buyCard(int supplyPos, gameState *state) {
    debug_printf("Entering buyCard...\n");

    // FIXME whatever mistake this references vvv
    // "I don't know what to do about the phase thing."

    int who = state->whoseTurn;

    if (state->numBuys < 1) {
        debug_printf("You do not have any buys left\n");

    } else if (supplyCount(supplyPos, state) <1) {
        debug_printf("There are not any of that type of card left\n");
    
    } else if (state->coins < getCost(supplyPos)) {
        debug_printf("You do not have enough money to buy that. You have %d coins.\n", state->coins);
    
    } else {
        state->phase=1;
        //state->supplyCount[supplyPos]--;
        gainCard(supplyPos, state, 0, who); //card goes in discard, this might be wrong.. (2 means goes into hand, 0 goes into discard)

        state->coins = (state->coins) - (getCost(supplyPos));
        state->numBuys--;

        debug_printf("You bought card number %d for %d coins. You now have %d buys and %d coins.\n", supplyPos, getCost(supplyPos), state->numBuys, state->coins);
        // state->discard[who][state->discardCount[who]] = supplyPos;
        // state->discardCount[who]++;
        return 0;
    }
    return -1;
}

int nextPlayer(gameState* state) {
    return (state->whoseTurn + 1)%state->numPlayers;
}

// TODO: inline?
int numHandCards(gameState *state) {
    return state->handCount[ state->whoseTurn ];
}

int handCard(int handPos, gameState *state) {
    return state->hand[state->whoseTurn][handPos];
}

int supplyCount(int card, gameState *state) {
    return state->supplyCount[card];
}

int fullDeckCount(int player, int card, gameState *state) {
    int count = 0;

    for (int i = 0; i < state->deckCount[player]; i++)
        if (state->deck[player][i] == card)
            count++;

    for (int i = 0; i < state->handCount[player]; i++)
        if (state->hand[player][i] == card)
            count++;

    for (int i = 0; i < state->discardCount[player]; i++)
        if (state->discard[player][i] == card)
            count++;

    return count;
}

// TODO remove currentPlayer binding
int endTurn(gameState *state) {
    int currentPlayer = state->whoseTurn;

    //Discard hand
    for (int i = 0; i < state->handCount[currentPlayer]; i++) {
        //Discard
        state->discard[currentPlayer][state->discardCount[currentPlayer]++] = state->hand[currentPlayer][i];
        //Set card to -1
        state->hand[currentPlayer][i] = -1;
    }

    state->handCount[currentPlayer] = 0; // Reset hand count

    // Code for determining the player
    if (currentPlayer < (state->numPlayers - 1)) { 
        // Still safe to increment
        state->whoseTurn = currentPlayer + 1;
    } else {
        //Max player has been reached, loop back around to player 1
        state->whoseTurn = 0;
    }

    state->outpostPlayed = 0;
    state->phase = 0;
    state->numActions = 1;
    state->coins = 0;
    state->numBuys = 1;
    state->playedCardCount = 0;
    state->handCount[state->whoseTurn] = 0;

    // int k; move to top
    // Next player draws hand
    for (int k = 0; k < 5; k++)
        drawCard(state->whoseTurn, state); // Draw a card

    // Update money
    updateCoins(state->whoseTurn, state , 0);

    return 0;
}

int isGameOver(gameState *state) {
    int i;
    int j;

    // if stack of Province cards is empty, the game ends
    if (state->supplyCount[province] == 0)
        return 1;

    // if three supply pile are at 0, the game ends
    j = 0;
    for (i = 0; i < 25; i++)
        if (state->supplyCount[i] == 0)
            j++;
    
    if ( j >= 3)
        return 1;

    return 0;
}

int scoreForHelper(gameState *state, int player, int count, int set[]) {
    int score = 0;

    for (int i=0; i < count; i++) {
        switch(set[i]) {
            case curse:
                score -= 1;
                break;

            case estate:
            case great_hall:
                score += 1;
                break;

            case duchy:
                score += 3;
                break;

            case province:
                score += 6;
                break;

            case gardens:
                score += (fullDeckCount(player, 0, state)/10);
                break;
        }
    }

    return score;
}

int scoreFor(int player, gameState *state) {
    int score = 0;

    // score from hand, discard, deck
    score += scoreForHelper(state, player, state->handCount[player], state->hand[player]);
    score += scoreForHelper(state, player, state->discardCount[player], state->discard[player]);
    score += scoreForHelper(state, player, state->discardCount[player], state->deck[player]);
    
    return score;
}

int getWinners(int players[MAX_PLAYERS], gameState *state) {
    // get score for each player
    for (int i = 0; i < MAX_PLAYERS; i++)
        // set unused player scores to INT_MIN
        if (i >= state->numPlayers) {
            players[i] = INT_MIN;
        } else {
            players[i] = scoreFor (i, state);
        }

    // find highest score
    int highScore = INT_MIN;
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (players[i] > highScore)
            highScore = players[i];

    // add 1 to players who had less turns
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (players[i] == highScore && i > state->whoseTurn)
            players[i]++;

    // find new highest score
    highScore = INT_MIN;
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (players[i] > highScore)
            highScore = players[i];

    // set winners in array to 1 and rest to 0
    for (int i = 0; i < MAX_PLAYERS; i++)
        players[i] = players[i] == highScore;

    return 0;
}

int drawCard(int player, gameState *state) {
    if (state->deckCount[player] <= 0) { //Deck is empty

        // Step 1 Shuffle the discard pile back into a deck
        // Move discard to deck
        for (int i = 0; i < state->discardCount[player];i++) {
            state->deck[player][i] = state->discard[player][i];
            state->discard[player][i] = -1;
        }

        state->deckCount[player] = state->discardCount[player];
        state->discardCount[player] = 0; // Reset discard

        // Shufffle the deck
        shuffle(player, state); // Shuffle the deck up and make it so that we can draw

        debug_printf("Deck count now: %d\n", state->deckCount[player]);

        state->discardCount[player] = 0;

        // Step 2 Draw Card
        int count = state->handCount[player]; // Get current player's hand count
        int deckCounter = state->deckCount[player]; // Create a holder for the deck count
        debug_printf("Current hand count: %d\n", count);

        if (!deckCounter) // exit if deck is empty
            return -1;

        state->hand[player][count] = state->deck[player][deckCounter - 1]; // Add card to hand
        state->deckCount[player]--;
        state->handCount[player]++; // Increment hand count
    }

    else {
        int count = state->handCount[player]; // Get current hand count for player
        int deckCounter = state->deckCount[player]; // Create holder for the deck count
        debug_printf("Current hand count: %d\n", count);

        state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to the hand
        state->deckCount[player]--;
        state->handCount[player]++; // Increment hand count
    }

    return 0;
}

int getCost(int cardNumber) {
    switch(cardNumber) {
        case curse:
        case copper:
            return 0;

        case estate:
        case embargo: 
            return 2;
        
        case silver:
        case ambassador:
        case steward:
            return 3;
        
        case feast:
        case gardens:
        case remodel:
        case smithy:
        case baron:
        case cutpurse:
        case salvager:
        case sea_hag:
        case treasure_map:
            return 4;
        
        case duchy:
        case council_room:
        case mine:
        case minion:
        case village:
        case great_hall:
        case tribute:
        case outpost:
            return 5;
        
        case gold:
        case adventurer:
            return 6;
        
        case province:
            return 8;
    }

    return -1;
}

// TODO Is bonus used at all?
int cardEffect(int card, int choice1, int choice2, int choice3, gameState *state, int handPos, int *bonus) {
    //uses switch to select card and perform actions
    switch(card) {
        case adventurer:    return adventurer_effect    (state);
        case council_room:  return council_room_effect  (state, handPos);
        case feast:         return feast_effect         (state,          choice1);
        case gardens:       return gardens_effect       ();
        case mine:          return mine_effect          (state, handPos, choice1, choice2);
        case remodel:       return remodel_effect       (state, handPos, choice1, choice2);
        case smithy:        return smithy_effect        (state, handPos);
        case village:       return village_effect       (state, handPos);
        case baron:         return baron_effect         (state,          choice1);
        case great_hall:    return great_hall_effect    (state, handPos);
        case minion:        return minion_effect        (state, handPos, choice1, choice2);
        case steward:       return steward_effect       (state, handPos, choice1, choice2, choice3);
        case tribute:       return tribute_effect       (state);
        case ambassador:    return ambassador_effect    (state, handPos, choice1, choice2);
        case cutpurse:      return cutpurse_effect      (state, handPos);
        case embargo:       return embargo_effect       (state, handPos, choice1);
        case outpost:       return outpost_effect       (state, handPos);
        case salvager:      return salvager_effect      (state, handPos, choice1);
        case sea_hag:       return sea_hag_effect       (state);
        case treasure_map:  return treasure_map_effect  (state, handPos);
        
        default:            return -1;
    }
}

// TODO: remove currentPlayer
int discardCard(int handPos, int currentPlayer, gameState *state, int trashFlag) {
    // if card is not trashed, added to Played pile 
    if (trashFlag < 1) {
        // add card to played pile
        state->playedCards[state->playedCardCount] = state->hand[currentPlayer][handPos]; 
        state->playedCardCount++;
    }

    // set played card to -1
    state->hand[currentPlayer][handPos] = -1;

    // TODO should be *currcount > 1?
    // remove card from player's hand
    int currCount = state->handCount[currentPlayer];
    int *currHand = state->hand[currentPlayer];
    
    if (handPos != (currCount - 1) && currCount != 1) { // if last card not played && not holding one card
        currHand[handPos] = currHand[(currCount - 1)];  // replace discarded card with last card in hand
        currHand[currCount - 1] = -1;                   // set last card to -1
    }

    state->handCount[currentPlayer]--;  // decrement hand card count

    return 0;
}

int gainCard(int supplyPos, gameState *state, int toFlag, int player) {
    //Note: supplyPos is enum of choosen card

    //check if supply pile is empty (0) or card is not used in game (-1)
    if ( supplyCount(supplyPos, state) < 1 )
        return -1;

    //added card for [whoseTurn] current player:
    // toFlag = 0 : add to discard
    // toFlag = 1 : add to deck
    // toFlag = 2 : add to hand

    switch(toFlag) {
        case 1:
            state->deck[ player ][ state->deckCount[player] ] = supplyPos;
            state->deckCount[player]++;
            break;

        case 2:
            state->hand[ player ][ state->handCount[player] ] = supplyPos;
            state->handCount[player]++;
            break;

        default:
            state->discard[player][ state->discardCount[player] ] = supplyPos;
            state->discardCount[player]++;
    }

    //decrease number in supply pile
    state->supplyCount[supplyPos]--;

    return 0;
}

int updateCoins(int player, gameState *state, int bonus) {
    int i;

    //reset coin count
    state->coins = 0;

    //add coins for each Treasure card in player's hand
    for (i = 0; i < state->handCount[player]; i++)
        switch(state->hand[player][i]) {
            case copper:
                state->coins += 1;
                break;

            case silver:
                state->coins += 2;
                break;

            case gold:
                state->coins += 3;
        }	

    //add bonus
    state->coins += bonus;

    return 0;
}


//end of dominion.c
