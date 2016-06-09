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
	printf("Starting cardtest1\n");
	//Default cards, as defined in playDom
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	struct gameState* p;
	int* bonus;
	
	//int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)

	//Call the games card effect function such that the desired card is being tested
	//int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)

	//Check all cases
	//adventurerCardEffect
	//Reveal cards from your deck until you reveal 2 Treasure cards. Put those Treasure cards in your hand and discard the other revealed cards.

	//Test when the treasure are at the top of the deck.

	printf("Test when both copper on top of deck.\n");
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = estate;
	p -> hand[0][1] = estate;
	p -> hand[0][2] = estate;
	p -> hand[0][3] = estate;
	p -> hand[0][4] = adventurer;
	p -> deck[0][0] = estate;
	p -> deck[0][1] = estate;
	p -> deck[0][2] = estate;
	p -> deck[0][3] = copper;
	p -> deck[0][4] = copper;
	p -> whoseTurn = 0;
	cardEffect(adventurer, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...
	assertEqual(p -> hand[0][4], copper, "The copper was not added.");
	assertEqual(p -> hand[0][5], copper, "The copper was not added.");
	assertEqual(p -> discardCount[0], 1, "The adventurer was not discarded.");
	assertEqual(p -> handCount[0], 6, "The hand size should increase by one.");

	//Check the case when the treasure are at the bottom of the deck
	printf("Test when both copper on the bottom of the deck.\n");
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = estate;
	p -> hand[0][1] = estate;
	p -> hand[0][2] = estate;
	p -> hand[0][3] = estate;
	p -> hand[0][4] = adventurer;
	p -> deck[0][0] = copper;
	p -> deck[0][1] = copper;
	p -> deck[0][2] = estate;
	p -> deck[0][3] = estate;
	p -> deck[0][4] = estate;
	p -> whoseTurn = 0;
	cardEffect(adventurer, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...
	assertEqual(p -> hand[0][4], copper, "The copper was not added.");
	assertEqual(p -> hand[0][5], copper, "The copper was not added.");
	assertEqual(p -> discardCount[0], 4, "The adventurer and estates should have been discarded.");
	assertEqual(p -> handCount[0], 6, "The hand size should increase by one.");

	//Check case when cards are in the middle
	printf("Test when both copper are in the middle of the deck.\n");
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = estate;
	p -> hand[0][1] = estate;
	p -> hand[0][2] = estate;
	p -> hand[0][3] = estate;
	p -> hand[0][4] = adventurer;
	p -> deck[0][0] = estate;
	p -> deck[0][1] = estate;
	p -> deck[0][2] = copper;
	p -> deck[0][3] = copper;
	p -> deck[0][4] = estate;
	p -> whoseTurn = 0;
	cardEffect(adventurer, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...
	assertEqual(p -> hand[0][4], copper, "The copper was not added.");
	assertEqual(p -> hand[0][5], copper, "The copper was not added.");
	assertEqual(p -> discardCount[0], 2, "The adventurer and an estate should have been discarded.");
	assertEqual(p -> handCount[0], 6, "The hand size should increase by one.");

	//Check case when we need to shuffle in discard pile to get the last card...
	printf("Test when we need to shuffle in discard pile to get the second copper.\n");
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = estate;
	p -> hand[0][1] = estate;
	p -> hand[0][2] = estate;
	p -> hand[0][3] = estate;
	p -> hand[0][4] = adventurer;
	p -> deck[0][0] = estate;
	p -> deck[0][1] = estate;
	p -> deck[0][2] = estate;
	p -> deck[0][3] = estate;
	p -> deck[0][4] = estate;
	p -> discard[0][0] = copper;
	p -> discard[0][1] = copper;
	p -> discardCount[0] = 2;
	p -> whoseTurn = 0;
	cardEffect(adventurer, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...
	assertEqual(p -> hand[0][4], copper, "The copper was not added.");
	assertEqual(p -> hand[0][5], copper, "The copper was not added.");
	assertEqual(p -> handCount[0], 6, "The hand size should increase by one.");

	//Check that if we only have one Treasure then the loop should end after one shuffle
	printf("Test when we only have one copper to get.\n");
	initializeGame(2,k,1,&g);
	p = &g;
	p -> hand[0][0] = estate;
	p -> hand[0][1] = estate;
	p -> hand[0][2] = estate;
	p -> hand[0][3] = estate;
	p -> hand[0][4] = adventurer;
	p -> deck[0][0] = estate;
	p -> deck[0][1] = estate;
	p -> deck[0][2] = estate;
	p -> deck[0][3] = copper;
	p -> deck[0][4] = estate;
	p -> whoseTurn = 0;
	cardEffect(adventurer, 0, 0, 0, p, 4, bonus);//pretty sure the choices dont matter...
	assertEqual(p -> hand[0][4], copper, "The copper was not added.");
	assertEqual(p -> handCount[0], 5, "The hand size should stay the same.");

	return 0;
}