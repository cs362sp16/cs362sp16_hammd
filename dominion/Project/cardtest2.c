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
	printf("Starting cardtest2\n");
	//Default cards, as defined in playDom
	int k[10] = {adventurer, council_room, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	struct gameState* p;
	int* bonus;
	
	//int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)

	//Council room
	//+4 cards
	//+1 buy
	//Each other player draws a card

	//Test for 2 players to 4 players
	for(int i = 2; i < 5; i++){
		printf("Test Council_Room for %d players.\n", i);
		initializeGame(i,k,1,&g);
		p = &g;

		//Make sure all players are initialized correctly
		//Decks, discard, hand and counters...
		for(int j = 0; j < i; j++){
			p -> hand[j][0] = copper;
			p -> hand[j][1] = copper;
			p -> hand[j][2] = copper;
			p -> hand[j][3] = copper;
			p -> hand[j][4] = council_room;
			p -> deck[j][0] = estate;
			p -> deck[j][1] = estate;
			p -> deck[j][2] = estate;
			p -> deck[j][3] = estate;
			p -> deck[j][4] = estate;
			p -> handCount[j] = 5;
			p -> deckCount[j] = 5;
			p -> discardCount[j] = 0;
		}
		p -> whoseTurn = 0;
		p -> numBuys = 1;

		//Play the card
		cardEffect(council_room, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...

		//Check for the expected outcomes
		assertEqual(p -> handCount[0], 8, "The hand size should increase by 3.");
		assertEqual(p -> deckCount[0], 1, "The deck size should decrease by four.");
		assertEqual(p -> discardCount[0], 1, "The council_room was not discarded.");
		assertEqual(p -> numBuys, 2, "The number of buys should increase by 1.");
		for(int j = 1; j < i; j++){
			assertEqual(p -> handCount[j], 6, "The hand size should increase by one.");
			assertEqual(p -> deckCount[j], 4, "The deck size should decrease by one.");
		}
	}

	return 0;
}