#include "dominion.h"
//Generates a random game state that is valid to test a player who is about to play an action card
//Note: after returning, will need to check the card location in order to play it...
struct gameState* generateRandomStateToTestActionCard(int actionCardToTest);