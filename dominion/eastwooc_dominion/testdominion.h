const char * const CARD_NAMES[] =
{"curse",
 "estate",
 "duchy",
 "province",

 "copper",
 "silver",
 "gold",

 "adventurer",
   /* If no/only 1 treasure found, stop when full deck seen */
 "council_room",
   "feast", /* choice1 is supply # of card gained) */
 "gardens",
   "mine", /* choice1 is hand# of money to trash, choice2 is supply# of
	    money to put in hand */
   "remodel", /* choice1 is hand# of card to remodel, choice2 is supply# */
   "smithy",
   "village",

   "baron", /* choice1: boolean for discard of estate */
   /* Discard is always of first (lowest index) estate */
   "great_hall",
   "minion", /* choice1:  1 = +2 coin, 2 = redraw */
   "steward", /* choice1: 1 = +2 card, 2 = +2 coin, 3 = trash 2 (choice2,3) */
   "tribute",

   "ambassador", /* choice1 = hand#, choice2 = number to return to supply */
   "cutpurse",
   "embargo", /* choice1 = supply# */
   "outpost",
   "salvager", /* choice1 = hand# to trash */
   "sea_hag",
   "treasure_map",
   "unknown"
 };


struct action {
	int handPos;
	int choice1;
	int choice2;
	int choice3;
};

void chooseKingdomCards(int *kingdomCards);

//void getCardName(char *card, int cardNumber);
//int getCost(int cardNumber);

struct action* newAction();
void determineNextActionToPlay(struct action *a, struct gameState *state);
int determineNextCardToBuy(struct gameState *state);

int setupPhase(struct gameState *state, int randomSeed);
int actionPhase(struct gameState *state);
int buyPhase(struct gameState *state);
void cleanupPhase(struct gameState *state);


