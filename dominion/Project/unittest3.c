#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <stdbool.h>

void assertTrue(bool statement, char* msg){
	if(statement != true){
		printf("Failed Assert: %s\n", msg);
	}
}

int main () {
	printf("Start unittest3 for kingdomCards()\n");
	//Test kingdomCards
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	int* cards = kingdomCards(k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9]);
	for(int i = 0; i < 10; i++){
		assertTrue(cards[i] == k[i], "Card array location missmatch");
	}
  	return 0;
}