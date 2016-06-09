#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <stdbool.h>

void assertEqual(int actualValue, int expectedValue, char* msg){
	if(actualValue != expectedValue ){
		printf("Failed Assert: %s:Expected(%d),Actual(%d)\n", msg, expectedValue, actualValue);
	}
}

int main () {
	printf("Start unittest2 for playCard()\n");
	//int playCard(int handPos, int choice1, int choice2, int choice3,struct gameState *state);

	int k[10] = {adventurer, council_room, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	struct gameState *p;
	



	//Play a card that gives actions and cards
	printf("Test playCard() with a card that gives actions and cards\n");
	//Setup the test
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = copper;
	p -> hand[0][1] = copper;
	p -> hand[0][2] = copper;
	p -> hand[0][3] = copper;
	p -> hand[0][4] = village;
	p -> deck[0][0] = copper;
	p -> deck[0][1] = copper;
	p -> deck[0][2] = copper;
	p -> deck[0][3] = copper;
	p -> deck[0][4] = copper;
	p -> handCount[0] = 5;
	p -> deckCount[0] = 5;
	p -> discardCount[0] = 0;
	p -> whoseTurn = 0;
	p -> numBuys = 1;
	p -> numActions = 1;
	p -> coins = 0;

	//execute method under test
	playCard(4, -1, -1, -1, p); //I dont think the choices matter

	//Check for expected values
	assertEqual(p -> handCount[0], 5, "Hand count should stay the same.");
	assertEqual(p -> discardCount[0], 1, "Discard count should increase by 1.");
	assertEqual(p -> deckCount[0], 4, "Deck count should decrease by 1.");
	assertEqual(p -> numActions, 2, "Actions should increase by 1.");
	assertEqual(p -> numBuys, 1, "Buys should stay the same.");
	assertEqual(p -> coins, 0, "Coins should stay the same.");


	//Play a card that gives buys
	printf("Test playCard() with a card that gives buys\n");
	//Setup the test
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = copper;
	p -> hand[0][1] = copper;
	p -> hand[0][2] = copper;
	p -> hand[0][3] = copper;
	p -> hand[0][4] = council_room;
	p -> deck[0][0] = copper;
	p -> deck[0][1] = copper;
	p -> deck[0][2] = copper;
	p -> deck[0][3] = copper;
	p -> deck[0][4] = copper;
	p -> handCount[0] = 5;
	p -> deckCount[0] = 5;
	p -> discardCount[0] = 0;
	p -> whoseTurn = 0;
	p -> numBuys = 1;
	p -> numActions = 1;
	p -> coins = 0;

	//execute method under test
	playCard(4, -1, -1, -1, p); //I dont think the choices matter

	//Check for expected values
	assertEqual(p -> handCount[0], 8, "Hand count should increase by 3.");
	assertEqual(p -> discardCount[0], 1, "Discard count should increase by 1.");
	assertEqual(p -> deckCount[0], 1, "Deck count should decrease by 4.");
	assertEqual(p -> numActions, 0, "Actions should decrease by 1.");
	assertEqual(p -> numBuys, 2, "Buys should increase by 1.");
	assertEqual(p -> coins, 0, "Coins should stay the same.");


	//Play a card that gives coins and requires a choice
	printf("Test playCard() with a card that gives coins\n");
	//Setup the test
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = copper;
	p -> hand[0][1] = copper;
	p -> hand[0][2] = copper;
	p -> hand[0][3] = copper;
	p -> hand[0][4] = steward;
	p -> deck[0][0] = copper;
	p -> deck[0][1] = copper;
	p -> deck[0][2] = copper;
	p -> deck[0][3] = copper;
	p -> deck[0][4] = copper;
	p -> handCount[0] = 5;
	p -> deckCount[0] = 5;
	p -> discardCount[0] = 0;
	p -> whoseTurn = 0;
	p -> numBuys = 1;
	p -> numActions = 1;
	p -> coins = 0;

	//execute method under test
	playCard(4, 2, -1, -1, p);

	//Check for expected values
	assertEqual(p -> handCount[0], 4, "Hand count decrease by 1.");
	assertEqual(p -> discardCount[0], 1, "Discard count should increase by 1.");
	assertEqual(p -> deckCount[0], 5, "Deck count should stay the same.");
	assertEqual(p -> numActions, 0, "Actions should decrease by 1.");
	assertEqual(p -> numBuys, 1, "Buys should stay the same.");
	assertEqual(p -> coins, 2, "Coins should increase by 2.");


	return 0;
}