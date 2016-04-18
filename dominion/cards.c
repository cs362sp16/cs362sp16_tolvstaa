#include "cards.h"
#include "dominion.h"
#include "dominion_helpers.h"
#include "debugprint.h"

int adventurer_effect(gameState* state) {
    int drawnTreasure=0;
    int tempHand[MAX_HAND], i=0;
    
    while(drawnTreasure<2) {
        // if the deck empty, shuffle discard and add to deck
        if(state->deckCount[state->whoseTurn]<1)
            shuffle(state->whoseTurn, state);
        
        drawCard(state->whoseTurn, state);
        
        // top card of hand is most recently drawn card.
        int drawn = state->hand[state->whoseTurn][state->handCount[state->whoseTurn]-1];
        if(drawn == copper || drawn == silver || drawn == gold) {
            drawnTreasure++;
        } else {
            tempHand[i]=drawn;
            state->handCount[state->whoseTurn]--; //remove top card
            i++;
        }
    }
    
    while(i>0) {
        // discard all cards in play that have been drawn
        state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] = tempHand[i-1];
        state->discardCount[state->whoseTurn]++;
        i--;
    }
    
    return 0;
}

int council_room_effect(gameState* state, int handPos) {
    // +4 Cards
    for(int i=0; i<4; i++)
        drawCard(state->whoseTurn, state);

    // +1 Buy
    state->numBuys++;

    // Each other player draws a card
    for(int i=0; i<state->numPlayers; i++)
        if (i != state->whoseTurn)
            drawCard(i, state);

    // put played card in played card pile
    discardCard(handPos, state->whoseTurn, state, 0);

    return 0;
}

int feast_effect(gameState* state, int choice1) {
    int temphand[MAX_HAND];
    // gain card with cost up to 5
    // Backup hand
    for (int i=0; i<=state->handCount[state->whoseTurn]; i++) {
        temphand[i] = state->hand[state->whoseTurn][i]; // Backup card
        state->hand[state->whoseTurn][i] = -1; // Set to nothing
    }
    // Backup hand

    // Update Coins for Buy
    updateCoins(state->whoseTurn, state, 5);

    int x = 1; // Condition to loop on
    while(x == 1) { // Buy one card
        if (supplyCount(choice1, state) <= 0){
                debug_printf("None of that card left, sorry!\n");
                debug_printf("Cards Left: %d\n", supplyCount(choice1, state));

        } else if (state->coins < getCost(choice1)){
            printf("That card is too expensive!\n");
            debug_printf("Coins: %d < %d\n", state->coins, getCost(choice1));
        
        } else {
            debug_printf("Deck Count: %d\n",
                state->handCount[state->whoseTurn] +
                state->deckCount[state->whoseTurn] +
                state->discardCount[state->whoseTurn]
            );

            gainCard(choice1, state, 0, state->whoseTurn); // Gain the card
            x = 0; // No more buying cards

            debug_printf("Deck Count: %d\n",
                state->handCount[state->whoseTurn] +
                state->deckCount[state->whoseTurn] +
                state->discardCount[state->whoseTurn]
            );
        }
    }     

    // Reset Hand
    for (int i=0; i <= state->handCount[state->whoseTurn]; i++) {
        state->hand[state->whoseTurn][i] = temphand[i];
        temphand[i] = -1;
    }
    // Reset Hand

    return 0;
}

int gardens_effect() {
    return -1;
}

int mine_effect(gameState* state, int handPos, int choice1, int choice2) {
    int j = state->hand[state->whoseTurn][choice1]; // store card we will trash

    if (state->hand[state->whoseTurn][choice1] < copper || state->hand[state->whoseTurn][choice1] > gold)
        return -1;

    if (choice2 > treasure_map || choice2 < curse)
        return -1;

    if ((getCost(state->hand[state->whoseTurn][choice1]) + 3) > getCost(choice2))
        return -1;

    gainCard(choice2, state, 2, state->whoseTurn);

    // discard card from hand
    discardCard(handPos, state->whoseTurn, state, 0);

    // discard trashed card
    for(int i = 0; i < state->handCount[state->whoseTurn]; i++)
        if (state->hand[state->whoseTurn][i] == j) {
            discardCard(i, state->whoseTurn, state, 0);			
            break;
        }
    return 0;
}

int remodel_effect(gameState* state, int handPos, int choice1, int choice2) {
    int j = state->hand[state->whoseTurn][choice1]; // store card we will trash
    
    if ((getCost(state->hand[state->whoseTurn][choice1]) + 2) > getCost(choice2) )
        return -1;

    gainCard(choice2, state, 0, state->whoseTurn);

    // discard card from hand
    discardCard(handPos, state->whoseTurn, state, 0);

    // discard trashed card
    for(int i = 0; i < state->handCount[state->whoseTurn]; i++)
        if (state->hand[state->whoseTurn][i] == j) {
            discardCard(i, state->whoseTurn, state, 0);			
            break;
        }
    
    return 0;
}

int smithy_effect(gameState* state, int handPos) {
    // +3 Cards
    for(int i=0; i<3; i++)
        drawCard(state->whoseTurn, state);

    // discard card from hand
    discardCard(handPos, state->whoseTurn, state, 0);
    return 0;
}

int village_effect(gameState* state, int handPos) {
    // +1 Card
    drawCard(state->whoseTurn, state);

    // +2 Actions
    state->numActions = state->numActions + 2;

    // discard played card from hand
    discardCard(handPos, state->whoseTurn, state, 0);
    return 0;
}

int baron_effect(gameState* state, int choice1) {
    state->numBuys++; // Increase buys by 1!
    if (choice1 > 0){ // Boolean true or going to discard an estate
        int p; // Iterator for hand!
        int card_not_discarded = 1; // Flag for discard set!
        while(card_not_discarded) {
            if (state->hand[state->whoseTurn][p] == estate) { // Estate card
                state->coins += 4; // Add 4 coins to the amount of coins
                state->discard[state->whoseTurn][state->discardCount[state->whoseTurn]] = state->hand[state->whoseTurn][p];
                state->discardCount[state->whoseTurn]++;
                
                for (p=0 ;p < state->handCount[state->whoseTurn]; p++)
                    state->hand[state->whoseTurn][p] = state->hand[state->whoseTurn][p+1];
                
                state->hand[state->whoseTurn][state->handCount[state->whoseTurn]] = -1;
                state->handCount[state->whoseTurn]--;
                card_not_discarded = 0; // Exit the loop
            } else if (p > state->handCount[state->whoseTurn]) {
                debug_printf("No estate cards in your hand, invalid choice\n");
                debug_printf("Must gain an estate if there are any\n");
                
                if (supplyCount(estate, state) > 0) {
                    gainCard(estate, state, 0, state->whoseTurn);
                    state->supplyCount[estate]--; // Decrement estates

                    if (supplyCount(estate, state) == 0)
                        isGameOver(state);

                }
                card_not_discarded = 0; // Exit the loop
            }

            else {
                p++; // Next card
            }
        }
    } else if (supplyCount(estate, state) > 0) {
        gainCard(estate, state, 0, state->whoseTurn); // Gain an estate
        state->supplyCount[estate]--; // Decrement Estates
        
        if (supplyCount(estate, state) == 0)
            isGameOver(state);
    }
    return 0;
}

int great_hall_effect(gameState* state, int handPos) {
    // +1 Card
    drawCard(state->whoseTurn, state);

    // +1 Actions
    state->numActions++;

    // discard card from hand
    discardCard(handPos, state->whoseTurn, state, 0);
    return 0;
}

int minion_effect(gameState* state, int handPos, int choice1, int choice2) {
    // +1 action
    state->numActions++;

    // discard card from hand
    discardCard(handPos, state->whoseTurn, state, 0);

    if (choice1) { 
        // +2 coins 
        state->coins = state->coins + 2;
    } else if (choice2) {
        // discard hand
        while(numHandCards(state) > 0) discardCard(handPos, state->whoseTurn, state, 0);

        // draw 4
        for (int i = 0; i < 4; i++) drawCard(state->whoseTurn, state);

        // other players discard hand and redraw if hand size > 4
        for (int i = 0; i < state->numPlayers; i++)
            if (i != state->whoseTurn)
                if ( state->handCount[i] > 4 ) {
                    // discard hand
                    while( state->handCount[i] > 0 )
                        discardCard(handPos, i, state, 0);

                    // draw 4
                    for (int j = 0; j < 4; j++)
                        drawCard(i, state);
                }
    }
    return 0;
}

int steward_effect(gameState* state, int handPos, int choice1, int choice2, int choice3) {
    if (choice1 == 1) {
        drawCard(state->whoseTurn, state); // +2 cards
        drawCard(state->whoseTurn, state);
    } else if (choice1 == 2) {
        state->coins = state->coins + 2; // +2 coins
    } else {
        discardCard(choice2, state->whoseTurn, state, 1); // trash 2 cards in hand
        discardCard(choice3, state->whoseTurn, state, 1);
    }

    // discard card from hand
    discardCard(handPos, state->whoseTurn, state, 0);
    return 0;
}

int tribute_effect(gameState* state) {
    int revealedCards[2] = {-1, 1};
    if ((state->discardCount[nextPlayer(state)] + state->deckCount[nextPlayer(state)]) <= 1) {
        if (state->deckCount[nextPlayer(state)] > 0){
            revealedCards[0] = state->deck[nextPlayer(state)][state->deckCount[nextPlayer(state)]-1];
            state->deckCount[nextPlayer(state)]--;

        } else if (state->discardCount[nextPlayer(state)] > 0) {
            revealedCards[0] = state->discard[nextPlayer(state)][state->discardCount[nextPlayer(state)]-1];
            state->discardCount[nextPlayer(state)]--;

        } else {
            debug_printf("No cards to reveal\n");
        }
    } else {
        if (state->deckCount[nextPlayer(state)] == 0) {
            for (int i=0; i < state->discardCount[nextPlayer(state)]; i++) {
                state->deck[nextPlayer(state)][i] = state->discard[nextPlayer(state)][i]; // Move to deck
                state->deckCount[nextPlayer(state)]++;
                state->discard[nextPlayer(state)][i] = -1;
                state->discardCount[nextPlayer(state)]--;
            }
            shuffle(nextPlayer(state), state); // Shuffle the deck
        }

        revealedCards[0] = state->deck[nextPlayer(state)][state->deckCount[nextPlayer(state)]-1];
        state->deck[nextPlayer(state)][state->deckCount[nextPlayer(state)]--] = -1;
        state->deckCount[nextPlayer(state)]--;

        revealedCards[1] = state->deck[nextPlayer(state)][state->deckCount[nextPlayer(state)]-1];
        state->deck[nextPlayer(state)][state->deckCount[nextPlayer(state)]--] = -1;
        state->deckCount[nextPlayer(state)]--;
    }    

    if (revealedCards[0] == revealedCards[1]) { // If we have a duplicate card, just drop one 
        state->playedCards[state->playedCardCount] = revealedCards[1];
        state->playedCardCount++;
        revealedCards[1] = -1;
    }

    for (int i=0; i<=2; i++){
        if (revealedCards[i] == copper || revealedCards[i] == silver || revealedCards[i] == gold) { // Treasure cards
            state->coins += 2;
        } else if (revealedCards[i] == estate || revealedCards[i] == duchy ||
                revealedCards[i] == province || revealedCards[i] == gardens ||
                revealedCards[i] == great_hall) { // Victory Card Found
            drawCard(state->whoseTurn, state);
            drawCard(state->whoseTurn, state);
        } else { // Action Card
            state->numActions = state->numActions + 2;
        }
    }
    return 0;
}

int ambassador_effect(gameState* state, int handPos, int choice1, int choice2) {
    int j = 0; // used to check if player has enough cards to discard

    if (choice2 > 2 || choice2 < 0) return -1;				
    if (choice1 == handPos) return -1;

    for (int i = 0; i < state->handCount[state->whoseTurn]; i++)
        if (i != handPos && i == state->hand[state->whoseTurn][choice1] && i != choice1)
            j++;
    
    if (j < choice2) return -1;				

    debug_printf("Player %d reveals card number: %d\n", state->whoseTurn, state->hand[state->whoseTurn][choice1]);

    // increase supply count for choosen card by amount being discarded
    state->supplyCount[state->hand[state->whoseTurn][choice1]] += choice2;

    // each other player gains a copy of revealed card
    for (int i = 0; i<state->numPlayers; i++)
        if (i != state->whoseTurn)
            gainCard(state->hand[state->whoseTurn][choice1], state, 0, i);

    // discard played card from hand
    discardCard(handPos, state->whoseTurn, state, 0);			

    // trash copies of cards returned to supply
    for (int i=0; i<choice2; i++)
        for (int k=0; j<state->handCount[state->whoseTurn]; k++)
            if (state->hand[state->whoseTurn][k] == state->hand[state->whoseTurn][choice1]) {
                discardCard(k, state->whoseTurn, state, 1);
                break;
            }

    return 0;
}

int cutpurse_effect(gameState* state, int handPos) {
    updateCoins(state->whoseTurn, state, 2);
    for (int i = 0; i < state->numPlayers; i++)
        if (i != state->whoseTurn)
            for (int j = 0; j < state->handCount[i]; j++) {
                if (state->hand[i][j] == copper) {
                    discardCard(j, i, state, 0);
                    break;
                }
                
                if (j == state->handCount[i]) {
                    for (int k = 0; k < state->handCount[i]; k++)
                        debug_printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
                    
                    break;
                }
            }

    // discard played card from hand
    discardCard(handPos, state->whoseTurn, state, 0);			

    return 0;
}

int embargo_effect(gameState* state, int handPos, int choice1) {
    // +2 Coins
    state->coins = state->coins + 2;

    // see if selected pile is in play
    if ( state->supplyCount[choice1] == -1 )
        return -1;

    // add embargo token to selected supply pile
    state->embargoTokens[choice1]++;

    // trash card
    discardCard(handPos, state->whoseTurn, state, 1);
    return 0;
}

int outpost_effect(gameState* state, int handPos) {
    // set outpost flag
    state->outpostPlayed++;

    // discard card
    discardCard(handPos, state->whoseTurn, state, 0);
    return 0;
}

int salvager_effect(gameState* state, int handPos, int choice1) {
    // +1 buy
    state->numBuys++;

    if (choice1) {
        //gain coins equal to trashed card
        state->coins = state->coins * getCost(handCard(choice1, state));
        //trash card
        discardCard(choice1, state->whoseTurn, state, 1);	
    }

    // discard card
    discardCard(handPos, state->whoseTurn, state, 0);
    return 0;
}

int sea_hag_effect(gameState* state) {
    for (int i=0; i<state->numPlayers; i++)
        if (i != state->whoseTurn) {
            state->discard[i][state->discardCount[i]] = state->deck[i][state->deckCount[i]--];
            state->deckCount[i]--;
            state->discardCount[i]++;
            state->deck[i][state->deckCount[i]--] = curse; // Top card now a curse
        }
        
    return 0;
}

int treasure_map_effect(gameState* state, int handPos) {
    // search hand for another treasure_map
    int index = -1;
    for (int i=0; i<state->handCount[state->whoseTurn]; i++)
        if (state->hand[state->whoseTurn][i] == treasure_map && i != handPos) {
            index = i;
            break;
        }
    
    if (index > -1) {
        // trash both treasure cards
        discardCard(handPos, state->whoseTurn, state, 1);
        discardCard(index, state->whoseTurn, state, 1);

        // gain 4 Gold cards
        for (int i = 0; i < 4; i++)
            gainCard(gold, state, 1, state->whoseTurn);

        // return success
        return 1;
    }

    // no second treasure_map found in hand
    return -1;
}
