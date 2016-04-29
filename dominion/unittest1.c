#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <stdbool.h>
//Test game initialization

void assertEqual(int actualValue, int expectedValue, char* msg){
	if(actualValue != expectedValue ){
		printf("Failed Assert: %s:Expected(%d),Actual(%d)\n", msg, expectedValue, actualValue);
	}
}

void testGameInitialization(int numberOfPlayers, int victoryCardTotal, int curseTotal){
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	initializeGame(numberOfPlayers,k,1,&g);
	struct gameState *p = &g;

	//Each player should have 5 drawn cards and 5 cards in deck
	for(int i = 0; i < numberOfPlayers; i++){
		assertEqual((p -> handCount[i]), 5, "Hand count wrong");
		assertEqual((p -> deckCount[i]), 5, "Deck count wrong");
		assertEqual((p -> discardCount[i]), 0, "Discard count wrong");
	}
	
	//The drawn and undrawn cards should combine to 7 copper and 3 estates
	int copperCount = 0;
	int estateCount = 0;
	for(int i = 0; i < numberOfPlayers; i++){
		//Initialize count
		copperCount = 0;
		estateCount = 0;

		//Search cards
		for(int j = 0; j < 5; j++){
			//search hand
			if(p->hand[i][j] == copper){
				copperCount++;
			} else if(p->hand[i][j] == estate){
				estateCount++;
			}
			//Search deck
			if(p->deck[i][j] == copper){
				copperCount++;
			} else if(p->deck[i][j] == estate){
				estateCount++;
			}
		}


		//Check results
		assertEqual(copperCount, 7, "Copper count");
		assertEqual(estateCount, 3, "Estate count");
	}

	//There should be 8 of each victory card and 10 curse cards
	assertEqual(p->supplyCount[estate], victoryCardTotal,"Estate supply");
	assertEqual(p->supplyCount[duchy], victoryCardTotal,"Duchy supply");
	assertEqual(p->supplyCount[province], victoryCardTotal,"Province supply");
	assertEqual(p->supplyCount[curse], curseTotal,"Curse supply");

	//There should be 10 of each non victory kingom card available and 8 of each victory kingom card available
	int CardTotal = 10;
	assertEqual(p->supplyCount[adventurer], CardTotal,"adventurer supply");
	assertEqual(p->supplyCount[gardens], victoryCardTotal,"gardens supply");
	assertEqual(p->supplyCount[embargo], CardTotal,"embargo supply");
	assertEqual(p->supplyCount[village], CardTotal,"village supply");
	assertEqual(p->supplyCount[minion], CardTotal,"minion supply");
	assertEqual(p->supplyCount[mine], CardTotal,"mine supply");
	assertEqual(p->supplyCount[cutpurse], CardTotal,"cutpurse supply");
	assertEqual(p->supplyCount[sea_hag], CardTotal,"sea_hag supply");
	assertEqual(p->supplyCount[tribute], CardTotal,"tribute supply");
	assertEqual(p->supplyCount[smithy], CardTotal,"smithy supply");
}

int main () {
	printf("Start unittest1 for initializeGame()\n");
	//Each player should have 10 cards
	//7 coppers
	//3 estates
	//Shuffle deck
	//Draw 5 cards

	//There should be 8 of each victory card and 10 curse cards for a 2 player game
	//There should be 12 of each victory card and 20 curse cards for a 3 player game
	//There should be 12 of each victory card and 30 curse cards for a 4 player game

	//Should be able to buy from 10 sets of kingdom cards
	//Each set of kingom cards has 10 cards
	//unless its a victory kingom card, to which it would have either 8 or 12 depending on number of players


	//Test 2 player game
	int numberOfPlayers = 2;
	int victoryCardTotal = 8;
	int curseTotal = 10;
	printf("Start 2 Player test\n");
	testGameInitialization(numberOfPlayers, victoryCardTotal, curseTotal);

	//Test 3 player game
	numberOfPlayers = 3;
	victoryCardTotal = 12;
	curseTotal = 20;
	printf("Start 3 Player test\n");
	testGameInitialization(numberOfPlayers, victoryCardTotal, curseTotal);

	//Test 4 player game
	numberOfPlayers = 4;
	curseTotal = 30;
	printf("Start 4 Player test\n");
	testGameInitialization(numberOfPlayers, victoryCardTotal, curseTotal);

	//Test starting a game with too many players
	numberOfPlayers = 5;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	printf("Start 5 Player test\n");
	assertEqual(initializeGame(numberOfPlayers, k, 1, &g), -1, "5 Player initialization");

	return 0;
}