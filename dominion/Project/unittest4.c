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
	printf("Start unittest4 for handCard()\n");
	//Test  whoseTurn

	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	struct gameState g;
	int numberOfPlayers = 4;
	initializeGame(numberOfPlayers,k,1,&g);
	struct gameState *p = &g;

	//Set the current turn for each player
	for(int i = 0; i < 4; i++){
		p -> whoseTurn = i;
		for(int j = 0; j < 5; j++){
			//Compare their cards to the found card
			assertEqual(handCard(j, p), p->hand[i][j], "Card missmatch");
		}
	}
	return 0;
}