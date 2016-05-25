struct action {
	int handPos;
	int choice1;
	int choice2;
	int choice3;
};

void chooseKingdomCards(int *kingdomCards);

void getCardName(char *card, int cardNumber);
int getCost(int cardNumber);

struct action* newAction();
void determineNextActionToPlay(struct action *a, struct gameState *state);
void determineNextCardToBuy(struct action *a, struct gameState *state);

void setupPhase(struct gameState *state, int randomSeed);
int actionPhase(struct gameState *state);
int buyPhase(struct gameState *state);
void cleanupPhase(struct gameState *state);