#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <stdbool.h>

//Test game initialization
int testGameInitialization(int numberOfPlayers, int victoryCardTotal, int curseTotal){
	int error = 0;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	initializeGame(numberOfPlayers,k,1,&g);
	struct gameState *p = &g;

	//Each player should have 5 drawn cards and 5 cards in deck
	for(int i = 0; i < numberOfPlayers; i++){
		if(p -> handCount[i] != 5){
			error = -1;
		}
		if(p -> deckCount[i] != 5){
			error = -1;
		}
		if(p -> discardCount[i] != 0){
			error = -1;
		}
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
		if(copperCount != 7){
			error = -1;
		}
		if(estateCount !=3){
			error = -1;
		}
	}

	//There should be 8 of each victory card and 10 curse cards
	if(p->supplyCount[estate] != victoryCardTotal){
		error = -1;
	}
	if(p->supplyCount[duchy] != victoryCardTotal){
		error = -1;
	}
	if(p->supplyCount[province] != victoryCardTotal){
		error = -1;
	}
	if(p->supplyCount[curse] != curseTotal){
		error = -1;
	}

	//There should be 10 of each non victory kingom card available and 8 of each victory kingom card available
	int CardTotal = 10;
	if(p->supplyCount[adventurer] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[gardens] != victoryCardTotal){
		error = -1;
	}
	if(p->supplyCount[embargo] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[village] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[minion] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[mine] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[cutpurse] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[sea_hag] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[tribute] != CardTotal){
		error = -1;
	}
	if(p->supplyCount[smithy] != CardTotal){
		error = -1;
	}

	return error;
}

int main(int argc, char *argv[]){
	int error = 0;

	//Test 2 player game
	int numberOfPlayers = 2;
	int victoryCardTotal = 8;
	int curseTotal = 10;
	if(testGameInitialization(numberOfPlayers, victoryCardTotal, curseTotal) == -1){
		error = -1;
	}

	//Test 3 player game
	numberOfPlayers = 3;
	victoryCardTotal = 12;
	curseTotal = 20;
	if(testGameInitialization(numberOfPlayers, victoryCardTotal, curseTotal) == -1){
		error = -1;
	}

	//Test 4 player game
	numberOfPlayers = 4;
	curseTotal = 30;
	if(testGameInitialization(numberOfPlayers, victoryCardTotal, curseTotal) == -1){
		error = -1;
	}

	//Test starting a game with too many players
	numberOfPlayers = 5;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	if(initializeGame(numberOfPlayers, k, 1, &g) == -1){
		error = -1;
	}

	return error;
}