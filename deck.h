#include "globalFunctions.h"
#include "card.h"
using namespace std;
#pragma once

class deck //This class is used for any group of cards, including what you would expect the deck to be or a player's hand
{
private:
	vector<card> cards;

public:
	void shuffle();

	deck(vector<card> cards);

	vector<card> getCards();

	card drawTopCard();

	void placeCardAtTop(card newCard);

	void showAll();

	void hideAll();

	void stack(int x, int y);

	void spreadVert(int x, int y);

	void spreadHoriz(int x, int y);

	int blackJackValue();
};
