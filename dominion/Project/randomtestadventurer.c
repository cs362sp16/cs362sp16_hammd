#include "dominion.h"
#include "randomStateGenerator.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

void assertEqual(int actualValue, int expectedValue, char* msg){
	if(actualValue != expectedValue ){
		printf("Failed Assert: %s:Expected(%d),Actual(%d)\n", msg, expectedValue, actualValue);
	}
}


int cardCount(int p[], int numCards, int card){
	int cardCount = 0;
	for(int i = 0; i < numCards; i++){
		if(p[i] == card){
			cardCount++;
		}
	}
	return cardCount;
}


int main (int argc, char** argv) {
	printf("Starting random test for adventurer\n");

	//Initilize random number generator
	time_t t;
	srand((unsigned) time(&t));

	struct gameState* p;
	
	//How many times to run test???
	for(int i = 0; i < 20; i++){
		//Generate the state
		p = generateRandomStateToTestActionCard(adventurer);

		//test card

		//Card Description:
		//Reveal cards from your deck until you reveal 2 Treasure cards. 
		//Put those Treasure cards in your hand and discard the other revealed cards
		//If you have to shuffle in the middle, shuffle. 
		//Don't shuffle in the revealed cards as these cards do not go to the Discard pile until you have finished revealing cards. 
		//If you run out of cards after shuffling and still only have one Treasure, you get just that one Treasure.

		//I will be using the following function to run the test
		//int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state);

		//first determine the location of the card after the random state was created
		int handPos = -1;
		int index = 0;
		while(handPos == -1){
			//This should never be called
			if(index == p -> handCount[p -> whoseTurn]){
				printf("Error in test...\n");
				return 1;
			}

			if(p -> hand[p -> whoseTurn][index] == adventurer){
				handPos = index;
			}
			index++;
		}

		//Set all necessary flags that will be used when testing
		int numberOfTreasureCardsInHandAtStart = 0;
		numberOfTreasureCardsInHandAtStart += cardCount(p -> hand[p -> whoseTurn], p -> handCount[p -> whoseTurn], copper);
		numberOfTreasureCardsInHandAtStart += cardCount(p -> hand[p -> whoseTurn], p -> handCount[p -> whoseTurn], silver);
		numberOfTreasureCardsInHandAtStart += cardCount(p -> hand[p -> whoseTurn], p -> handCount[p -> whoseTurn], gold);

		int totalTreasures = numberOfTreasureCardsInHandAtStart;
		totalTreasures += cardCount(p -> deck[p -> whoseTurn], p -> deckCount[p -> whoseTurn], copper);
		totalTreasures += cardCount(p -> deck[p -> whoseTurn], p -> deckCount[p -> whoseTurn], silver);
		totalTreasures += cardCount(p -> deck[p -> whoseTurn], p -> deckCount[p -> whoseTurn], gold);
		totalTreasures += cardCount(p -> discard[p -> whoseTurn], p -> discardCount[p -> whoseTurn], copper);
		totalTreasures += cardCount(p -> discard[p -> whoseTurn], p -> discardCount[p -> whoseTurn], silver);
		totalTreasures += cardCount(p -> discard[p -> whoseTurn], p -> discardCount[p -> whoseTurn], gold);

		int numberOfTreasureCardsToBeAdded = 0;
		if((totalTreasures - numberOfTreasureCardsInHandAtStart) >= 2){
			numberOfTreasureCardsToBeAdded = 2;
		} else {
			numberOfTreasureCardsToBeAdded = (totalTreasures - numberOfTreasureCardsInHandAtStart);
		}

		//Look at how many cards in the deck would come before finding two treasure cards
		int minNumberOfCardsToBeRevealed = 0;
		int numTreasuresFound = 0;
		index = 0;
		while(numTreasuresFound < 2 && index < p -> deckCount[p -> whoseTurn]){
			int card = p -> deck[p -> whoseTurn][index];
			if(card == copper || card == silver || card == gold){
				numTreasuresFound++;
			}else{
				minNumberOfCardsToBeRevealed++;
			}
			index++;
		}

		//This will either be the previous discardCount + minNumberOfCardsToBeRevealed
		//or just minNumberOfCardsToBeRevealed

		int numberOfTreasureCardsInHandAfter = numberOfTreasureCardsInHandAtStart + numberOfTreasureCardsToBeAdded;

		int numberOfBuysAfter = p -> numBuys;
		int numberOfCoinsAfter = p -> coins;
		int numberOfActionsAfter = (p -> numActions) - 1;
		int whoseTurnAfter = p -> whoseTurn;
		int gamePhaseAfter = p -> phase;
		int handCountAfter = (p -> handCount[whoseTurnAfter]) - 1 + numberOfTreasureCardsToBeAdded;
		int deckCountAfter = (p -> deckCount[whoseTurnAfter]) - (minNumberOfCardsToBeRevealed + numberOfTreasureCardsToBeAdded);
		int discardCountAfter = (p -> discardCount[whoseTurnAfter]) + 1 + minNumberOfCardsToBeRevealed;


		//Run function
		//No choices needed for this card...
		playCard(handPos, 0, 0, 0, p);


		//Check for expected outcomes
		//Check that the hand has the correct number of treasures
		int treasureCountAfter = 0;
		treasureCountAfter += cardCount(p -> hand[whoseTurnAfter], p -> handCount[whoseTurnAfter], copper);
		treasureCountAfter += cardCount(p -> hand[whoseTurnAfter], p -> handCount[whoseTurnAfter], silver);
		treasureCountAfter += cardCount(p -> hand[whoseTurnAfter], p -> handCount[whoseTurnAfter], gold);
		assertEqual(treasureCountAfter, numberOfTreasureCardsInHandAfter, "Player's hand does not contain the correct number of treasures.");

		//Check that the player owns has the correct number of treasures
		//hand, discard, deck...
		treasureCountAfter += cardCount(p -> deck[whoseTurnAfter], p -> deckCount[whoseTurnAfter], copper);
		treasureCountAfter += cardCount(p -> deck[whoseTurnAfter], p -> deckCount[whoseTurnAfter], silver);
		treasureCountAfter += cardCount(p -> deck[whoseTurnAfter], p -> deckCount[whoseTurnAfter], gold);
		treasureCountAfter += cardCount(p -> discard[whoseTurnAfter], p -> discardCount[whoseTurnAfter], copper);
		treasureCountAfter += cardCount(p -> discard[whoseTurnAfter], p -> discardCount[whoseTurnAfter], silver);
		treasureCountAfter += cardCount(p -> discard[whoseTurnAfter], p -> discardCount[whoseTurnAfter], gold);
		assertEqual(treasureCountAfter, totalTreasures, "Player's total treasure count has changed.");

		//Check buys
		assertEqual(p -> numBuys, numberOfBuysAfter, "Player's buy count has changed.");

		//Check coins
		assertEqual(p -> coins, numberOfCoinsAfter, "Player's coin count has changed.");

		//Check actions
		assertEqual(p -> numActions, numberOfActionsAfter, "Player's action count is wrong.");

		//Check whose turn
		assertEqual(p -> whoseTurn, whoseTurnAfter, "WhoseTurn  has changed.");

		//Check gamePhase
		assertEqual(p -> phase, gamePhaseAfter, "GamePhase count has changed.");

		//Check handCount
		assertEqual(p -> handCount[whoseTurnAfter], handCountAfter, "handCount is wrong");

		//Check deckCount and discardCount if no shuffle was required
		//If we didnt shuffle then it should be the old value
		if(p -> deckCount[whoseTurnAfter] > minNumberOfCardsToBeRevealed + numberOfTreasureCardsToBeAdded){
			assertEqual(p -> deckCount[whoseTurnAfter], deckCountAfter, "deckCount is wrong");
			assertEqual(p -> discardCount[whoseTurnAfter], discardCountAfter, "discardCount is wrong");
		}

		//dealocate memory
		free(p);
	}

	return 0;
}