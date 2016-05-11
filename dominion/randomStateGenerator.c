#include "dominion.h"
#include "randomStateGenerator.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

//Generates a random game state that is valid to test a player who is about to play an action card
//Note: after returning, will need to check the card location in order to play it...
struct gameState* generateRandomStateToTestActionCard(int actionCardToTest){
	//Initilize the game
	struct gameState* g = malloc(sizeof(struct gameState));
	//There can be 2 to 4 players
	g -> numPlayers = (rand() % 3) + 2;

	//Initialize supplyCount and embargoTokens to all -1
	for(int i = 0; i < treasure_map; i++){
		g -> supplyCount[i] = -1;
		g -> embargoTokens[i] = -1;
	}

	//Initialize all hands decks and discards to -1
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 30; j++){
			g -> hand[i][j] = -1;
			g -> deck[i][j] = -1;
			g -> discard[i][j] = -1;
		}
	}

	//Initialize playedCards to -1
	for(int j = 0; j < 30; j++){
		g -> playedCards[j] = -1;
	}

	//Initialize card supplies
	//curses
	if(g -> numPlayers == 2)
	{
		g -> supplyCount[curse] = 10;
	}
	else if(g -> numPlayers == 3)
	{
		g -> supplyCount[curse] = 20;
	}
	else
	{
		g -> supplyCount[curse] = 30;
	}

	//victory cards
	if(g -> numPlayers == 2)
	{
		g -> supplyCount[estate] = 8;
		g -> supplyCount[duchy] = 8;
		g -> supplyCount[province] = 8;
	}
	else
	{
		g -> supplyCount[estate] = 12;
		g -> supplyCount[duchy] = 12;
		g -> supplyCount[province] = 12;
	}

	//treasure cards
	g -> supplyCount[copper] = 60 - (7 * (g -> numPlayers));
	g -> supplyCount[silver] = 40;
	g -> supplyCount[gold] = 30;


	//Now to include all kingdom cards while accounting for actionCardToTest
	//First add the actionCardToTest to the supply
	int* kingdomCards = malloc(10 * sizeof(int));
	kingdomCards[0] = actionCardToTest;

	//Next create an array of all other possible cards
	int* possibleCards = malloc((treasure_map - adventurer - 1) * sizeof(int));

	//Fill in the possible cards
	int index = 1;
	for(int i = adventurer; i <= treasure_map; i++){
		if(i!=actionCardToTest){
			possibleCards[index] = i;
			index++;
		}
	}

	//Finally draw 9 cards from the array and add them to the supply
	for(int i = 1; i<10; i++){
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

		//Dealocate possibleCards
		//for(int n = 0; n<sizeof(possibleCards); n++){
		//	free(possibleCards[n]);
		//}
		free(possibleCards);

		//Create a new possibleCards array from the temp arary
		possibleCards = malloc(sizeof(temp) * sizeof(int));
		for(int n = 0; n<sizeof(possibleCards); n++){
			possibleCards[n] = temp[n];
		}

		//Dealocate the temp array
		//for(int n = 0; n<sizeof(temp); n++){
		//	free(temp[n]);
		//}
		free(temp);
	}

	//Dealocate possibleCards
	//for(int n = 0; n<sizeof(possibleCards); n++){
	//	free(possibleCards[n]);
	//}
	free(possibleCards);

	//Set the supply counts for all chosen cards
	for (int i = adventurer; i <= treasure_map; i++)       	//loop all cards
	{
	    for (int j = 0; j < 10; j++)           		//loop chosen cards
	    {
	    	if (kingdomCards[j] == i)
	        {
		    	//check if card is a 'Victory' Kingdom card
	         	if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
	         	{
	          		if (g -> numPlayers == 2)
	          		{ 
	            		g -> supplyCount[i] = 8; 
	          		}
	          		else{ g -> supplyCount[i] = 12; }
	        	}
	        	else
	        	{
	        		if(i == actionCardToTest){
	        			g -> supplyCount[i] = 9;
	        		}else{
	        			g -> supplyCount[i] = 10;
	        		}
	        	}
	        	break; //There can only be one of each type of card
	      	}
	 	}
	}

	//Dealocate space from kingdom cards
	free(kingdomCards);

	//There is some specific logic regarding embargoTokens
	//Embargo tokens are placed when a player plays the Embargo Card...
	//Cant decide until after the card has been included in the game...
	//int embargoTokens[treasure_map+1];


	//outpostPlayed and outpostTurn track the behavior of the card Outpost
	//The initialization for theses values should follow the correct logic
	g -> outpostPlayed = rand() % 3;

	//Depends on outpostPlayed
	if(g -> outpostPlayed == 0){
		g -> outpostTurn = 0;
	} else if(g -> outpostPlayed == 2){
		g -> outpostTurn = 1;
	} else {
		g -> outpostTurn = rand() % 2;
	}



	//Choose the player that will test the kingomCard
	g -> whoseTurn = (rand() % (g -> numPlayers));

	//The phase required to play a card is 0
	g -> phase = 0;

	//These values are determined by the initial and what cards have been played...
	//The player must have at least 1 action to play their card
	//Limit to 10 for now. Would be better to know what could actually be possible within the game. Monte carlo to find out?
	(g -> numActions) = ((rand() % 10) + 1);

	//TODO: Determine max possble coin value to limit rand to realistic values....
	g -> coins = (rand() % 10);

	//TODO: Determine max possble numBuys value to limit rand to realistic values....
	g -> numBuys = (rand() % 10) + 1;


	//Create a deck for each player
	for(int i = 0; i < g -> numPlayers; i++){
		g -> deckCount[i] = rand() % 30 + 1;

		//Fill the hand
		for(int j = 0; j < g -> deckCount[i]; j++){
			//Pick from supply
			//If the supply is empty, then move up one, if its the last item, then circle back
			int cardAvailable = 0;
			int index = rand() % treasure_map;
			while(cardAvailable==0){
				if(g -> supplyCount[index] > 0){
					g -> deck[i][j] = index;
					g -> supplyCount[index]--;
					cardAvailable = 1; 
				}
				index++;
				if(index > treasure_map){
					index = 0;
				}
			}
		}
	}

	//Create a hand for each player
	for(int i = 0; i < g -> numPlayers; i++){
		//Determing number of cards
		g -> handCount[i] = rand() % 10;
		//Fill the hand
		for(int j = 0; j < g -> handCount[i]; j++){
			//Pick from supply
			//If the supply is empty, then move up one, if its the last item, then circle back
			int cardAvailable = 0;
			int index = rand() % treasure_map;
			while(cardAvailable==0){
				if(g -> supplyCount[index] > 0){
					g -> hand[i][j] = index;
					g -> supplyCount[index]--;
					cardAvailable = 1; 
				}
				index++;
				if(index > treasure_map){
					index = 0;
				}
			}
		}
	}
	//Ensure that the active player has the test card in there hand
	//Replace one card with the desired card
	if(g -> handCount[g -> whoseTurn] != 0){
		int cardIndexToReplace = (rand() % (g -> handCount[g -> whoseTurn]));
		int cardToPutbackIntoSupply = g -> hand[g -> whoseTurn][cardIndexToReplace];
		g -> hand[g -> whoseTurn][cardIndexToReplace] = actionCardToTest;
		//update the supply
		(g -> supplyCount[cardToPutbackIntoSupply])++;
	} else{
		(g -> handCount[g -> whoseTurn])++;
		g -> hand[g -> whoseTurn][0] = actionCardToTest;
	}
	

	//Create a discard pile for each player
	for(int i = 0; i < g -> numPlayers; i++){
		//Determine how many cards from the deck should be put into the discard pile
		g -> discardCount[i] = rand() % (g -> deckCount[i]);

		//Remove cards from deck and put them into the discard pile
		for(int j = 0; j < g -> discardCount[i]; j++){
			g -> discard[i][j] = g -> deck[i][(g -> deckCount[i]) - j - 1];//I think a -1 is needed for the deck when removing from back..
		} 

		//update deckcount
		(g -> deckCount[i]) -= g -> discardCount[i];
	}


	//int playedCards[MAX_DECK];
	//This should just be for the active player I think...

	//Not sure how to use this atm...
	g -> playedCardCount = 0;

	return g;
}