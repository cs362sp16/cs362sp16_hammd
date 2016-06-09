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
	printf("Starting random test for steward\n");

	//Initilize random number generator
	time_t t;
	srand((unsigned) time(&t));

	struct gameState* p;
	
	//Used to find the action card under test in the players hand
	int handPos;
	int index;

	//Test player attributes
	int numberOfBuysAfter;
	int numberOfCoinsAfter;
	int numberOfActionsAfter;

	//Test game settings
	int whoseTurnAfter;
	int gamePhaseAfter;

	//Test card manipulation
	int handCountAfter;
	int deckCountAfter;
	int discardCountAfter;

	//Cards to trash
	int choice2;
	int choice3;

	//Card Description:
	//Choose one: +2 Cards; or +2 Coins; or trash 2 cards from your hand
	//If the player has less then two cards in their hand when they try to trash just trash the reamaining cards
	//ie: if only one card then trash just that card

	//I will be using the following function to run the test
	//int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state);


	//////////////////////
	//Test option of +2 cards
	//////////////////////
	for(int i = 0; i < 20; i++){
		printf("Starting test for +2 cards\n");

		//Generate the state
		p = generateRandomStateToTestActionCard(steward);

		//Determine the location of the card after the random state was created
		handPos = -1;
		index = 0;
		while(handPos == -1){
			//This should never be called
			//Because the generated state should always create a state with the required action card in hand...
			if(index == p -> handCount[p -> whoseTurn]){
				printf("Error in test finding: (%d) with card count(%d)\n", steward, p -> handCount[p->whoseTurn]);
				return 1;
			}

			if(p -> hand[p -> whoseTurn][index] == steward){
				handPos = index;
			}
			index++;
		}

		//Set all necessary expected values
		numberOfBuysAfter = p -> numBuys;
		numberOfCoinsAfter = p -> coins;
		numberOfActionsAfter = (p -> numActions) - 1;
		whoseTurnAfter = p -> whoseTurn;
		gamePhaseAfter = p -> phase;
		int possibleDrawCount = (p -> deckCount[whoseTurnAfter] + p -> discardCount[whoseTurnAfter]);
		handCountAfter = (possibleDrawCount > 2)?((p -> handCount[whoseTurnAfter]) + 1):((p -> handCount[whoseTurnAfter]) - 1 + possibleDrawCount);
		//Check cards in deck
		switch(p -> deckCount[whoseTurnAfter]){
			case 0:
				if(p -> discardCount[whoseTurnAfter] >= 2){
					deckCountAfter = (p -> discardCount[whoseTurnAfter]) - 2;
					discardCountAfter = 0;
				}else{
					deckCountAfter = 0;
					discardCountAfter = 0;
				}
				break;
			case 1:
				if(p -> discardCount[whoseTurnAfter] > 0){
					deckCountAfter = (p -> discardCount[whoseTurnAfter]) - 1;
					discardCountAfter = 0;
				}
				break;
			default:
				deckCountAfter = 0;
				discardCountAfter = (p -> discardCount[whoseTurnAfter]);
		}
		if((p -> deckCount[whoseTurnAfter]) > 2){
			deckCountAfter = (p -> deckCount[whoseTurnAfter]) - 2;
			discardCountAfter = 0;
		}


		//Run function
		playCard(handPos, 1, 0, 0, p);


		//Check for expected outcomes
		//Check buys
		assertEqual(p -> numBuys, numberOfBuysAfter, "Player's buy count is wrong.");

		//Check coins
		assertEqual(p -> coins, numberOfCoinsAfter, "Player's coin count is wrong");

		//Check actions
		assertEqual(p -> numActions, numberOfActionsAfter, "Player's action count is wrong.");

		//Check whose turn
		assertEqual(p -> whoseTurn, whoseTurnAfter, "WhoseTurn is wrong.");

		//Check gamePhase
		assertEqual(p -> phase, gamePhaseAfter, "GamePhase count is wrong");

		//Check handCount
		assertEqual(p -> handCount[whoseTurnAfter], handCountAfter, "handCount is wrong");

		//Check deckCount
		assertEqual(p -> deckCount[whoseTurnAfter], deckCountAfter, "deckCount is wrong");

		//Check discardCount
		assertEqual(p -> discardCount[whoseTurnAfter], discardCountAfter, "discardCount is wrong");


		//dealocate memory
		free(p);
	}

	//////////////////////
	//Test option of +2 coins
	//////////////////////
	for(int i = 0; i < 20; i++)
	{
		printf("Starting test for +2 coins\n");

		//Generate the state
		p = generateRandomStateToTestActionCard(steward);

		//Determine the location of the card after the random state was created
		handPos = -1;
		index = 0;
		while(handPos == -1){
			//This should never be called
			//Because the generated state should always create a state with the required action card in hand...
			if(index == p -> handCount[p -> whoseTurn]){
				printf("Error in test finding: (%d) with card count(%d)\n", steward, p -> handCount[p->whoseTurn]);
				return 1;
			}

			if(p -> hand[p -> whoseTurn][index] == steward){
				handPos = index;
			}
			index++;
		}

		//Set all necessary expected values
		numberOfBuysAfter = p -> numBuys;
		numberOfCoinsAfter = p -> coins + 2;
		numberOfActionsAfter = (p -> numActions) - 1;
		whoseTurnAfter = p -> whoseTurn;
		gamePhaseAfter = p -> phase;
		handCountAfter = p -> handCount[whoseTurnAfter] - 1;
		discardCountAfter = p -> discardCount[whoseTurnAfter];
		deckCountAfter = p -> deckCount[whoseTurnAfter];


		//Run function
		playCard(handPos, 2, 0, 0, p);


		//Check for expected outcomes
		//Check buys
		assertEqual(p -> numBuys, numberOfBuysAfter, "Player's buy count is wrong.");

		//Check coins
		assertEqual(p -> coins, numberOfCoinsAfter, "Player's coin count is wrong");

		//Check actions
		assertEqual(p -> numActions, numberOfActionsAfter, "Player's action count is wrong.");

		//Check whose turn
		assertEqual(p -> whoseTurn, whoseTurnAfter, "WhoseTurn is wrong.");

		//Check gamePhase
		assertEqual(p -> phase, gamePhaseAfter, "GamePhase count is wrong");

		//Check handCount
		assertEqual(p -> handCount[whoseTurnAfter], handCountAfter, "handCount is wrong");

		//Check deckCount
		assertEqual(p -> deckCount[whoseTurnAfter], deckCountAfter, "deckCount is wrong");

		//Check discardCount
		assertEqual(p -> discardCount[whoseTurnAfter], discardCountAfter, "discardCount is wrong");


		//dealocate memory
		free(p);
	}

	//////////////////////
	//Test option of trash two cards
	//////////////////////
	for(int i = 0; i < 20; i++)
	{
		printf("Starting test trash two cards\n");

		//Generate the state
		p = generateRandomStateToTestActionCard(steward);

		//Determine the location of the card after the random state was created
		handPos = -1;
		index = 0;
		while(handPos == -1){
			//This should never be called
			//Because the generated state should always create a state with the required action card in hand...
			if(index == p -> handCount[p -> whoseTurn]){
				printf("Error in test finding: (%d) with card count(%d)\n", steward, p -> handCount[p->whoseTurn]);
				return 1;
			}

			if(p -> hand[p -> whoseTurn][index] == steward){
				handPos = index;
			}
			index++;
		}

		//Set all necessary expected values
		numberOfBuysAfter = p -> numBuys;
		numberOfCoinsAfter = p -> coins + 2;
		numberOfActionsAfter = (p -> numActions) - 1;
		whoseTurnAfter = p -> whoseTurn;
		gamePhaseAfter = p -> phase;
		discardCountAfter = p -> discardCount[whoseTurnAfter];
		deckCountAfter = p -> deckCount[whoseTurnAfter];

		if(p -> handCount[whoseTurnAfter] <= 3){
			handCountAfter = 0;			
		} else{
			handCountAfter = p -> handCount[whoseTurnAfter] - 3;

		}

		//determine what cards to trash
		switch(handPos)
		{
			case 0:
				choice2 = 1;
				choice3 = 2;
				break;
			case 1:
				choice2 = 0;
				choice3 = 2;
				break;
			default:
				//should be a random value below handPos
				choice2 = rand() % handPos;

				//find a value for choice3 that does not conflict with choice2
				if(choice2 == 0){
					choice3 = rand() % (handPos - 1) + 1;
				} else if (choice2 == (handPos - 1))
				{
					choice3 = rand() % choice2;
				} else{
					choice3 = rand() % choice2;
					//if both choices are the same then increment or decrement choice3 by 1
					if(choice3 == choice2){
						if(rand() % 2){
							choice3++;
						}else{
							choice3--;
						}
					}
				}
				break;
		}

		//Run function
		playCard(handPos, 3, choice2, choice3, p);


		//Check for expected outcomes
		//Check buys
		assertEqual(p -> numBuys, numberOfBuysAfter, "Player's buy count is wrong.");

		//Check coins
		assertEqual(p -> coins, numberOfCoinsAfter, "Player's coin count is wrong");

		//Check actions
		assertEqual(p -> numActions, numberOfActionsAfter, "Player's action count is wrong.");

		//Check whose turn
		assertEqual(p -> whoseTurn, whoseTurnAfter, "WhoseTurn is wrong.");

		//Check gamePhase
		assertEqual(p -> phase, gamePhaseAfter, "GamePhase count is wrong");

		//Check handCount
		assertEqual(p -> handCount[whoseTurnAfter], handCountAfter, "handCount is wrong");

		//Check deckCount
		assertEqual(p -> deckCount[whoseTurnAfter], deckCountAfter, "deckCount is wrong");

		//Check discardCount
		assertEqual(p -> discardCount[whoseTurnAfter], discardCountAfter, "discardCount is wrong");


		//dealocate memory
		free(p);
	}

	return 0;
}