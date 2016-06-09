#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

void assertEqual(int actualValue, int expectedValue, char* msg){
	if(actualValue != expectedValue ){
		printf("Failed Assert: %s:Expected(%d),Actual(%d)\n", msg, expectedValue, actualValue);
	}
}

int main (int argc, char** argv) {
	printf("Starting cardtest4\n");
	//Default cards, as defined in playDom
	int k[10] = {adventurer, council_room, great_hall, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	struct gameState* p;
	int* bonus;

	//Great Hall
	//+1 card
	//+1 actions

	printf("Test Great Hall card effect.\n");
	//Setup the test
	initializeGame(2,k,1,&g);
	p = &g;
	p -> whoseTurn = 0;
	p -> numBuys = 1;
	p -> numActions = 1;
	p -> hand[0][0] = copper;
	p -> hand[0][1] = copper;
	p -> hand[0][2] = copper;
	p -> hand[0][3] = copper;
	p -> hand[0][4] = great_hall;
	p -> deck[0][0] = estate;
	p -> deck[0][1] = estate;
	p -> deck[0][2] = estate;
	p -> deck[0][3] = estate;
	p -> deck[0][4] = estate;
	p -> handCount[0] = 5;
	p -> deckCount[0] = 5;
	p -> discardCount[0] = 0;

	//Play the card
	cardEffect(great_hall, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...

	//Check for the expected values
	assertEqual(p -> handCount[0], 5, "The hand size should stay the same.");
	assertEqual(p -> deckCount[0], 4, "The deck size decrease by 1.");
	assertEqual(p -> discardCount[0], 1, "The discard size should increase by 1.");
	assertEqual(p -> numActions, 2, "The number of actions should increase by 1.");

	return 0;
}