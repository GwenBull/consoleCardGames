#include "globalFunctions.h"
#include "card.h"
using namespace std;
#pragma once

class deck //This class is used for any group of cards, including what you would expect the deck to be or a player's hand
{
private:
	vector<card> cards;

public:
	void shuffle() {
		for (int k = 0; k < this->cards.size(); k++) {
			unsigned int seed = static_cast<unsigned int>(time(NULL)); // forces the current time to be an integer
			srand(seed); //sets that integer as the seed for randomisation
			int r = k + rand() % (this->cards.size() - k); //uses the above seed to select a random card within this deck
			swap(this->cards[k], this->cards[r]); //swaps the card at current iterator position with the randomly
		}
	}

	deck(vector<card> cards);
};