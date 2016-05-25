#include "dominion.h" //will want to only include the dominion class that is being played...
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include "testdominion.h"

//need to reuse the setting of cards and players from my random tester
//this needs to take in a dominion game object to tester

//Will need to create an agent to act as a player(s)

//Takes in one argument to be used for the random seed
int main (int argc, char** argv) {
	//Use the the given players gamestruct here 
	struct gameState* g = newGame();

	//Setup phase
	setupPhase(g, 100);

 	//Loop through core game phases until end conditions are met
 	int gameNotOver = 1;
 	while(gameNotOver){
 		//Action phase
 		gameNotOver = actionPhase(g);

 		//Buy phase
 		gameNotOver = buyPhase(g);

 		//Clean-up phase
 		if(gameNotOver){
			cleanupPhase(g);
		}
 	}

 	//Determine and output the games conclusion
 	printf("The game has concluded with players recieving the following scores:\n");
 	for(int i = 0; i < g -> numPlayers; i++){
 		printf("Player%d:(%d)\n", i + 1, scoreFor(i, g));
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
	int cost = 0;
	for(int i = 0; i < (state -> handCount[whoseTurn(state)]); i++){
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
				a -> handPos = i;
				//needs choices
				//Trash a treasure card from your hand, gain a treasure card costing up to 3 more
				a -> choice1 = rand() % state -> handCount[whoseTurn(state)];
				a -> choice2 = (rand() % 3) + 4;
				break;
			case remodel:
				a -> handPos = i;
				//needs choices
				//Trash a card from your hand. Gain a card costing up to 2 more than the trashed card
				a -> choice1 = rand() % state -> handCount[whoseTurn(state)];
				a -> choice2 = (rand() % treasure_map) + 1;
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
				a -> handPos = i;
				//needs choices
				//Reveal a card from your hand. Return up to 2 copies of it from your hand to the Supply. Then each other player gains a copy of it.
				a -> choice1 = rand() % state -> handCount[whoseTurn(state)];
				a -> choice2 = rand() % state -> handCount[whoseTurn(state)];
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
				a -> handPos = i;
				break;
		}
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

int* chooseKingdomCards(int *kingdomCards){
	//Determine what kingdom cards to use
	int* kingdomCards = malloc(10 * sizeof(int));

	//Next create an array of all other possible cards
	int* possibleCards = malloc((treasure_map - adventurer - 1) * sizeof(int));

	//Fill in the possible cards
	int index = 0;
	for(int i = adventurer; i <= treasure_map; i++){
		possibleCards[index] = i;
		index++;
	}

	//Finally draw 10 cards from the array and add them to the supply
	for(int i = 0; i<10; i++){
		//Randomly select a possilbe card
		int choice = rand() % sizeof(possibleCards);

		//Add that card to the kingdomcards array
		kingdomCards[i] = possibleCards[choice];

		//Copy all cards besides the selected to a temp array
		int* temp = malloc((sizeof(possibleCards) - 1) * sizeof(int));
		index = 0;
		for(int j = 0; j <= (sizeof(possibleCards)); j++){
			if(possibleCards[j]!=kingdomCards[i]){
				temp[index] = possibleCards[j];
				index++;
			}
		}
		free(possibleCards);

		//Create a new possibleCards array from the temp arary
		possibleCards = malloc(sizeof(temp) * sizeof(int));
		for(int n = 0; n<sizeof(possibleCards); n++){
			possibleCards[n] = temp[n];
		}
		free(temp);
	}
	free(possibleCards);

	return kingdomcards;
}

void setupPhase(struct gameState *state, int randomSeed){
	//Determine the number of players
	int playerCount = (rand() % 3) + 2;

	//Determine what kingdom cards should be used
	int *kingdomCards;
	chooseKingdomCards(kingdomCards);

	//initialize the game through initializGame() which should be implemented such that the game completes the setup phase...
	//Could test this
 	initializeGame(playerCount, kingdomcards, randomSeed, state);
}

int actionPhase(struct gameState *state){
	int gameNotOver = 1;
	int playerEndsPhase = 0;
	printf("Player %d has started their action phase with %d actions, %d coins, and %d buys.\n", whoseTurn(state), state -> numActions, state -> coins, state -> numBuys);
	while(state -> numActions > 0 && !playerEndsPhase){
		//How would I go about deciding what action cards the player would want to use and when???
		//int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state) 

		//Determin players next action card to play
		struct action* a = newAction();
		determineNextAction(a, state); 

		//If a player chooses to not play an action, then the card to play should be -1
		if(a -> handPos != -1){
			char *card;
			getCardName(card, state -> hand[whoseTurn(state)][a -> handPos]);

			//Could run my test here
			int error = playCard(a -> handPos, a -> choice1, a -> choice2, a -> choice3, state);
			if(error == -1){
				printf("Error trying to play:(%s) with the following choices: (choice1: %d) (choice2: %d) (choice3: %d)\n", card, a -> choice1, a -> choice2, a -> choice3);
			} else{
				printf("%s was played with the following choices: (choice1: %d) (choice2: %d) (choice3: %d)", card, a -> choice1, a -> choice2, a -> choice3);
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
	printf("Player %d has ended their action phase with %d actions, %d coins, and %d buys.\n", whoseTurn(state), state -> numActions, state -> coins, state -> numBuys);
 	return gameNotOver;
}

int buyPhase(struct gameState *state){
	//The first step should be to decide the order in which a player plays treasure cards
	//However, the provided implementation converts treasure cards during other steps...

	//Loop while player can still buy something
	//According to the rules, a player could choose not to buy something
	int gameNotOver = 1;
	int playerEndsPhase = 0;
	int coinsBeforeBuy = state -> coins;
	printf("Player%d has started their buy phase with %d coins and %d buys.\n", whoseTurn(state), coinsBeforeBuy, state -> numBuys);
	while(gameNotOver && state -> numBuys > 0 && !playerEndsPhase){
		//Determine players next card to buy
		int supplyPos = determineNextCardToBuy(g);
		char *card;
		getCardName(card, supplyPos);

		//supplyPos of -1 means the player chooses not to buy another card
		if(supplyPos != -1){
			int error = buyCard(supplyPos, g)
			if(error == -1){
				printf("Error trying to buy:(%s)\n", card);
			} else{
				printf("Player %d bought (%s) for %d coins.\n", whoseTurn(state), card, coinsBeforeBuy - state -> coins);
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
	printf("Player%d has ended their buy phase with %d coins and %d buys remaining.\n", whoseTurn(state), state -> coins, state -> numBuys);
	return gameNotOver;
}



void cleanupPhase(struct gameState *state){
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
	int currentPlayer = state -> whoseTurn;
	if(state -> outpostPlayed > 0 || state -> outpostTurn == 0){
		endTurn(state);
		state -> outpostTurn = 1;
		state -> whoseTurn = currentPlayer;
	} else{
		endTurn(state);
		state -> outpostTurn = 0;
	}
}

void getCardName(char *card, int cardNumber){
	switch(cardNumber){
		case curse:
			card = "Curse";
			break;
		case estate:
			card = "Estate";
			break;
		case duchy:
			card = "Duchy";
			break;
		case province:
			card = "Province";
			break;
		case copper:
			card = "Copper";
			break;
		case silver:
			card = "Silver";
			break;
		case gold:
			card = "Gold";
			break;
		case adventurer:
			card = "Adventurer";
			break;
		case council_room:
			card = "Council Room";
			break;
		case feast:
			card = "Feast";
			break;
		case gardens:
			card = "Gardens";
			break;
		case mine:
			card = "Mine";
			break;
		case remodel:
			card = "Remodel";
			break;
		case smithy:
			card = "Smithy";
			break;
		case village:
			card = "Village";
			break;
		case baron:
			card = "Baron";
			break;
		case great_hall:
			card = "Great Hall";
			break;
		case minion:
			card = "Minion";
			break;
		case steward:
			card = "Steward";
			break;
		case tribute:
			card = "Tribute";
			break;
		case ambassador:
			card = "Ambassador";
			break;
		case cutpurse:
			card = "Cutpurse";
			break;
		case embargo:
			card = "Embargo";
			break;
		case outpost:
			card = "Outpost";
			break;
		case salvager:
			card = "Salvager";
			break;
		case sea_hag:
			card = "Sea Hag";
			break;
		case treasure_map:
			card = "Treasure Map";
			break;
	}
}

int getCost(int cardNumber)
{
  switch( cardNumber ) 
  {
    case curse:
    return 0;
    case estate:
    return 2;
    case duchy:
    return 5;
    case province:
    return 8;
    case copper:
    return 0;
    case silver:
    return 3;
    case gold:
    return 6;
    case adventurer:
    return 6;
    case council_room:
    return 5;
    case feast:
    return 4;
    case gardens:
    return 4;
    case mine:
    return 5;
    case remodel:
    return 4;
    case smithy:
    return 4;
    case village:
    return 3;
    case baron:
    return 4;
    case great_hall:
    return 3;
    case minion:
    return 5;
    case steward:
    return 3;
    case tribute:
    return 5;
    case ambassador:
    return 3;
    case cutpurse:
    return 4;
    case embargo: 
    return 2;
    case outpost:
    return 5;
    case salvager:
    return 4;
    case sea_hag:
    return 4;
    case treasure_map:
    return 4;
  }
  
  return -1;
}