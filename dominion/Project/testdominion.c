#include "dominion.h" //will want to only include the dominion class that is being played...
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "testdominion.h"
#include <string.h>

//need to reuse the setting of cards and players from my random tester
//this needs to take in a dominion game object to tester

//Will need to create an agent to act as a player(s)

//Takes in one argument to be used for the random seed
int main (int argc, char** argv) {
 	int gameNotOver = 1;
	printf("\n");
	//Use the the given players gamestruct here 
	struct gameState* g = malloc(sizeof(struct gameState));
	time_t t;
	srand((unsigned) time(&t));

	//Setup phase
	gameNotOver = !setupPhase(g, rand() % 32000);

 	//Loop through core game phases until end conditions are met
 	//for(int i = 0; i < 400; i++){
 	while(gameNotOver){
 		//Action phase
 		gameNotOver = actionPhase(g);

 		//Buy phase
 		if(gameNotOver == 1){
 			gameNotOver = buyPhase(g);
 		}

 		//Clean-up phase
 		if(gameNotOver == 1){
			printf("Player%d has started their cleanup phase\n", whoseTurn(g));
			//cleanupPhase(g);
			endTurn(g);
		} else{
			break;
		}
	}
 	//Determine and output the games conclusion
 	printf("The game has concluded with players recieving the following scores:\n");
 	for(int i = 0; i < g -> numPlayers; i++){
 		printf("Player%d:(%d)\n", i, scoreFor(i, g));
 	}
 	printf("\n");

 	printf("End game values:\n");
 	int card;
 	for(int i = 0; i < g -> numPlayers; i++){
 		printf("Player %d has %d cards in hand, %d cards in discard, %d cards in deck\n", i, g -> handCount[i], g -> discardCount[i], g -> deckCount[i]);
 		for(int j = 0; j < g -> handCount[i]; j++){	 
 			card = g -> hand[i][j];		
	 		printf("Hand[%d] = %s\n", j, CARD_NAMES[card]);
 		}
 		for(int j = 0; j < g -> discardCount[i]; j++){	 
 			card = g -> discard[i][j];		
	 		printf("discard[%d] = %s\n", j, CARD_NAMES[card]);
 		}
 		for(int j = 0; j < g -> deckCount[i]; j++){	 	
 			card = g -> deck[i][j];	
	 		printf("deck[%d] = %s\n", j, CARD_NAMES[card]);
 		}
 	}

 	printf("\nCard supply info:\n");
 	for(int i = 0; i < treasure_map; i++){
 		printf("There are %d of %s\n", g -> supplyCount[i], CARD_NAMES[i]);
 	}
}


struct action* newAction(){
	struct action* a = malloc(sizeof(struct action));
	return a;
}

//Will need to copy gameState multiple times...
void determineNextActionToPlay(struct action *a, struct gameState *state){
	//Might want to search though a players full turn...
	//Need to loop though a players cards to see what action cards they could play
	int checkCount = 0;
	int index = 0;
	int copperIndex = -1;
	int silverIndex = -1;
	int silverAvailable = 0;
	int goldAvailable = 0;
	int duplicateCheck = 0;
	//int cost = 0;
	int i = 0;
	a -> handPos = -1;
	a -> choice1 = -1;
	a -> choice2 = -1;
	a -> choice3 = -1;
	int numberOfCards = state -> handCount[whoseTurn(state)];
	while(i < numberOfCards && a -> handPos == -1){
		switch(state -> hand[whoseTurn(state)][i]){
			case adventurer:
				a -> handPos = i;
				break;
			case council_room:
				a -> handPos = i;
				break;
			case feast:
				a -> handPos = i;
				//needs choices
				//Gain a card costing up to 5
				index = (rand() % treasure_map) + 1;
				a -> choice1 = 0;
				while((a -> choice1 == 0) && (checkCount < (treasure_map - 1))){
					if(getCost(index) <= 5 && state -> supplyCount[index] > 0){
						a -> choice1 = index;
					} else{
						index++;
						if(index > treasure_map){
							index = 1;
						}
					}
					checkCount++;
				}
				break;
			case mine:
				//Does player have a silver?
				for(int j = 0; j < numberOfCards; j++){
					if(state -> hand[whoseTurn(state)][j] == silver){
						silverIndex = j;
					}
				}

				//is a gold is available
				if(state -> supplyCount[gold] > 0){
					goldAvailable = 1;
				}

				//Does player have a copper?
				for(int j = 0; j < numberOfCards; j++){
					if(state -> hand[whoseTurn(state)][j] == copper){
						copperIndex = j;
					}
				}

				//is a silver is available
				if(state -> supplyCount[silver] > 0){
					silverAvailable = 1;
				}

				//pick one of the two options
				if(copperIndex != -1 && silverIndex != -1 && silverAvailable == 1 && goldAvailable == 1){
					if((rand() % 2) == 0){
						a -> choice1 = copperIndex;
						a -> choice2 = silver;
					} else{
						a -> choice1 = silverIndex;
						a -> choice2 = gold;
					}
					a -> handPos = i;
				} else if(silverIndex != -1 && goldAvailable == 1){
					a -> choice1 = silverIndex;
					a -> choice2 = gold;
					a -> handPos = i;
				} else if(copperIndex != -1 && silverAvailable == 1){
					a -> choice1 = copperIndex;
					a -> choice2 = silver;
					a -> handPos = i;
				}
				break;
			case remodel:
				//needs choices
				//Trash a card from your hand. Gain a card costing up to 2 more than the trashed card
				//Find a card that could be trashed
				for(int n = 0; n < numberOfCards; n++){
					if(n != i){
						index = rand() % (treasure_map + 1);
						//find a card that could be gained
						for(int j = 0; j <= treasure_map; j++){
							//Pick a card to trash
							//Check if there is a card to gain
							if(state -> supplyCount[index] > 0 &&  getCost(index) <= (getCost(state -> hand[whoseTurn(state)][n]) + 2)){
								a -> choice1 = n;
								a -> choice2 = index;
								break;
							}
						}
					}
					if(a -> choice1 != -1){
						a -> handPos = i;
						break;
					}
				}
				break;
			case smithy:
				a -> handPos = i;
				break;
			case village:
				a -> handPos = i;
				break;
			case baron:
				a -> handPos = i;
				//needs choices
				//You may discard an estate card
				a -> choice1 = rand() % state -> handCount[whoseTurn(state)];
				break;
			case great_hall:
				a -> handPos = i;
				break;
			case minion:
				a -> handPos = i;
				//needs choices
				//Coose one:  +$2; or discard your hand, +4 Cards; and each other player with at least 5 cards in hand discards his hand and draws 4 cards.
				a -> choice1 = (rand() % 2) + 1;
				break;
			case steward:
				a -> handPos = i;
				//needs choices
				//Choose one: +2 Cards; or +$2; or trash 2 cards from your hand.
				a -> choice1 = (rand() % 3) + 1;
				if(a -> choice1 == 3){
					a -> choice2 = rand() % state -> handCount[whoseTurn(state)];
					a -> choice3 = rand() % state -> handCount[whoseTurn(state)];
					if(a -> choice3 == a -> choice2){
						a -> choice2 = ((a -> choice2) - 1);
						if(a -> choice2 == 0){
							a -> choice2 = state -> handCount[whoseTurn(state)] - 1;
						}
					}
				}
				break;
			case tribute:
				a -> handPos = i;
				break;
			case ambassador:
				//We will only use this card to remove curses for now...
				//needs choices
				//Reveal a card from your hand. Return up to 2 copies of it from your hand to the Supply. Then each other player gains a copy of it.

				//Choice1 is the card to reveal, which cant be this card
				//Choice2 is the number of those cards to discard which must be 1 or 2
				//The player must have the correct number of those cards to discard
				index = -1;
				for(int j = 0; j < numberOfCards; j++){
					if(i != j && state -> hand[j] == curse){
						index = j;
						//Check if the player has 2 cards
						duplicateCheck = 0;
						for(int n = 0; n < numberOfCards; n++){
							if(n != i && n != j && state -> hand[n] == state -> hand[j]){
								duplicateCheck++;
							}
							if(duplicateCheck == 2){
								break;
							}
						}
						break;
					}
				}
				if(index != -1 && duplicateCheck != 0){
					a -> handPos = i;
					a -> choice1 = index;
					a -> choice2 = duplicateCheck;
				}
				break;
			case cutpurse:
				a -> handPos = i;
				break;
			case embargo:
				a -> handPos = i;
				break;
			case outpost:
				a -> handPos = i;
				break;
			case salvager:
				a -> handPos = i;
				//needs choices
				//Trash a card from your hand. +$ equal to its cost.
				a -> choice1 = rand() % state -> handCount[whoseTurn(state)];
				break;
			case sea_hag:
				a -> handPos = i;
				break;
			case treasure_map:
				//Check if the player has a second treasure map
				index = -1;
				for(int j = 0; j < numberOfCards; j++){
					if(i != j){
						if(state -> hand[whoseTurn(state)][j] == treasure_map){
							index = j;
						}
					}
				}
				if(index != -1){
					a -> handPos = i;
					a -> choice1 = index;
				}
				break;
		}
		i++;
	}
}

int determineNextCardToBuy(struct gameState *state){
	int supplyPos = -1;
	int checkCount = 0;
	int index = (rand() % treasure_map) + 1;
	while((supplyPos == -1) && (checkCount < (treasure_map - 1))){
		if(getCost(index) <= state -> coins && state -> supplyCount[index] > 0){
			supplyPos = index;
		} else{
			index++;
			if(index > treasure_map){
				index = 1;
			}
		}
		checkCount++;
	}

	return supplyPos;
}

void chooseKingdomCards(int *kingdomCards){
	//Determine what kingdom cards to use

	//Create an array of all other possible cards
	int possibleCardsSize = 1 + treasure_map - adventurer;
	int* possibleCards = malloc(possibleCardsSize * sizeof(int));

	//Fill in the possible cards
	int index = 0;
	//adventurer = 7
	//treasure_map = 26
	for(int i = adventurer; i <= treasure_map; i++){
		possibleCards[index] = i;
		index++;
	}

	//Finally draw 10 cards from the array and add them to the supply
	for(int i = 0; i<10; i++){
		//Randomly select a possilbe card
		int choice = rand() % possibleCardsSize;

		//Add that card to the kingdomcards array
		kingdomCards[i] = possibleCards[choice];

		//Copy all cards besides the selected to a temp array
		int tempSize = possibleCardsSize - 1;
		int* temp = malloc(tempSize * sizeof(int));
		index = 0;
		for(int j = 0; j < possibleCardsSize; j++){
			if(kingdomCards[i] != possibleCards[j]){
				temp[index] = possibleCards[j];
				index++;
			}
		}
		free(possibleCards);

		//Create a new possibleCards array from the temp arary
		possibleCardsSize = tempSize;
		possibleCards = malloc(possibleCardsSize * sizeof(int));
		for(int n = 0; n < possibleCardsSize; n++){
			possibleCards[n] = temp[n];
		}
		free(temp);
	}
	free(possibleCards);
}

int setupPhase(struct gameState *state, int randomSeed){
	int card;
	//Determine the number of players
	int playerCount = (rand() % 3) + 2;

	//Determine what kingdom cards should be used
	int* kingdomCards = malloc(10 * sizeof(int));
	for(int i = 0; i < 10; i++){
		kingdomCards[i] = 0;
	}
	chooseKingdomCards(kingdomCards);
	for(int i = 0; i < 10; i++){
		card = kingdomCards[i];
		printf("kingdomCard[%d] = %s\n", i, CARD_NAMES[card]);
	}

	//initialize the game through initializGame() which should be implemented such that the game completes the setup phase...
 	int error = initializeGame(playerCount, kingdomCards, randomSeed, state);

 	printf("The game has been initialized with the following values:\n");
 	printf("Player count: %d\n", state -> numPlayers);
 	for(int i = 0; i < state -> numPlayers; i++){
 		printf("Player %d has %d cards in hand, %d cards in discard, %d cards in deck\n", i, state -> handCount[i], state -> discardCount[i], state -> deckCount[i]);
 		for(int j = 0; j < state -> handCount[i]; j++){	 
 			card = state -> hand[i][j];		
	 		printf("Hand[%d] = %s\n", j, CARD_NAMES[card]);
 		}
 		for(int j = 0; j < state -> discardCount[i]; j++){	 
 			card = state -> discard[i][j];		
	 		printf("discard[%d] = %s\n", j, CARD_NAMES[card]);
 		}
 		for(int j = 0; j < state -> deckCount[i]; j++){	 	
 			card = state -> deck[i][j];	
	 		printf("deck[%d] = %s\n", j, CARD_NAMES[card]);
 		}
 	}



 	printf("\nCard supply info:\n");
 	for(int i = 0; i < treasure_map; i++){
 		printf("There are %d of %s\n", state -> supplyCount[i], CARD_NAMES[i]);
 	}

 	if(error == 1){
 		printf("================================================================\n");
 		printf("Error during initializeGame()\n");
 		printf("================================================================\n");
 	}
 	return error;
}

int actionPhase(struct gameState *state){
	int error = 0;
	int gameNotOver = 1;
	int playerEndsPhase = 0;
	printf("\nPlayer %d has started their action phase with %d actions, %d coins, and %d buys.\n", whoseTurn(state), state -> numActions, state -> coins, state -> numBuys);
	while(state -> numActions > 0 && !playerEndsPhase){
		//Determin players next action card to play
		struct action* a = newAction();
		determineNextActionToPlay(a, state); 

		//If a player chooses to not play an action, then the card to play should be -1
		if(a -> handPos != -1){
			int card = state -> hand[whoseTurn(state)][a -> handPos];

			error = playCard(a -> handPos, a -> choice1, a -> choice2, a -> choice3, state);
			if(error == -1){
				printf("================================================================\n");
				printf("Error trying to play:(%s) with the following choices: (choice1: %d) (choice2: %d) (choice3: %d)\n", CARD_NAMES[card], a -> choice1, a -> choice2, a -> choice3);
				printf("================================================================\n");
				playerEndsPhase = 1;
			} else{
				printf("%s was played with the following choices: (choice1: %d) (choice2: %d) (choice3: %d)\n", CARD_NAMES[card], a -> choice1, a -> choice2, a -> choice3);
				printf("Player %d has %d actions, %d coins, and %d buys left.\n", whoseTurn(state), state -> numActions, state -> coins, state -> numBuys);
			}
		} else{
			playerEndsPhase = 1;
		}

		//Free action memory
		free(a);

		//Check if the game ended as a result of an action card being played
		gameNotOver = !isGameOver(state);
		if(!gameNotOver){
			break;
		}
	}
	if(error == 1){
		gameNotOver = 0;
	} else{
		printf("Player %d has ended their action phase with %d actions, %d coins, and %d buys.\n", whoseTurn(state), state -> numActions, state -> coins, state -> numBuys);
 	}
	return gameNotOver;
}

int buyPhase(struct gameState *state){
	int error = 0;
	//The first step should be to decide the order in which a player plays treasure cards
	//However, the provided implementation converts treasure cards during other steps...
	//Had to add this method to dominion.h
	updateCoins(whoseTurn(state), state, 0);

	//Loop while player can still buy something
	//According to the rules, a player could choose not to buy something
	int gameNotOver = 1;
	int playerEndsPhase = 0;
	int coinsBeforeBuy = state -> coins;
	printf("\nPlayer%d has started their buy phase with %d coins and %d buys.\n", whoseTurn(state), coinsBeforeBuy, state -> numBuys);
	while(gameNotOver && state -> numBuys > 0 && !playerEndsPhase){
		//Determine players next card to buy
		int supplyPos = determineNextCardToBuy(state);
		int card = supplyPos;

		//supplyPos of -1 means the player chooses not to buy another card
		if(supplyPos != -1){
			error = buyCard(supplyPos, state);
			if(error == -1){
				printf("================================================================\n");
				printf("Error trying to buy:(%s)\n", CARD_NAMES[card]);
				printf("================================================================\n");
				playerEndsPhase = 1;
			} else{
				printf("Player %d bought (%s) for %d coins.\n", whoseTurn(state), CARD_NAMES[card], coinsBeforeBuy - state -> coins);
				printf("Player %d has %d buys left.\n", whoseTurn(state), state -> numBuys);
				coinsBeforeBuy = state -> coins;
			}
		} else{
			playerEndsPhase = 1;
		}

		//Check if the game ended as a result of a player buying a card
		gameNotOver = !isGameOver(state);
		if(!gameNotOver){
			break;
		}
	}
	printf("Player%d has ended their buy phase with %d coins and %d buys remaining.\n\n", whoseTurn(state), state -> coins, state -> numBuys);
	if(error == 1){
		gameNotOver = 0;
	}
	return gameNotOver;
}



void cleanupPhase(struct gameState *state){
	printf("\n\nPlayer%d has started their cleanup phase\n\n", whoseTurn(state));
	//The following logic should be implemented inside endTurn() but since its not for the provided implementation
	//Ive included it for now
	/*
		I will assume that outpostTurn only gets updated in the cleanup phase
		Basically, if an outpost was played, then during cleanup outpostTurn becomes 1
		This way, on the next cleanup phase, outpostPlayed == 1 signifies that the player must end their turn
	*/
 	//Note: the current implementation for endTurn() does not draw the correct number of cards
 	//I'm going to leave it as is for now in case someone fixed it
 	//The other logic shouldn't be a problem...
	/*
	int currentPlayer = state -> whoseTurn;
	if(state -> outpostPlayed > 0 || state -> outpostTurn == 0){
		endTurn(state);
		state -> outpostTurn = 1;
		state -> whoseTurn = currentPlayer;
	} else{
		endTurn(state);
		state -> outpostTurn = 0;
	}
	*/
	endTurn(state);
}