#include "Deck.h"
#include "GlobalFunctions.h"
using namespace std;

Deck::Deck(vector<Card> cards) {
	this->cards = cards;
}

vector<Card> Deck::getCards() { 
	return this->cards; //returns the entire set of cards in this Deck/hand
}

Card Deck::drawTopCard() {
	Card drawnCard = this->cards[this->cards.size() - 1]; //copies the back Card
	this->cards.pop_back(); //removes the back Card from the Deck
	return drawnCard; //returns the Card that has been removed
	//because Card rendering iterates forwards through each Deck,
	//the last indexed item is the one drawn at the top, so I am
	//treating it as such.
	//basically: the "top" of the Deck, is the "back" of the vector.
	//Wait that's just a stack :p
}

void Deck::placeCardAtTop(Card newCard) {
	this->cards.push_back(newCard); //puts a new Card at the top of the Deck
}

void Deck::shuffle() {
	for (int i = 0; i < this->cards.size(); i++) { //iterates through the Deck
		unsigned int seed = static_cast<unsigned int>(time(NULL)); //generates a seed (current time as int)
		srand(seed); //puts that seed into the built in random function
		int r = i + rand() % (this->cards.size() - i); //selects a random Card in the Deck
		swap(this->cards[i], this->cards[r]); //swaps Card at current iteration with
	}
}

void Deck::showAll() {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setFace(true); //sets every Card in this Deck to face up
	}
}

void Deck::hideAll() {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setFace(false); //sets every Card in this Deck to face down
	}
}

void Deck::flipSpecific(int which, bool face) {
	this->cards[which].setFace(face);
}

void Deck::stack(int x, int y) {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y); //places this entire Deck at the specified position
	}
}

void Deck::spreadVert(int x, int y) {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y + (i * 2)); //distributes the Deck vertically from a specified position
	}
}

void Deck::spreadHoriz(int x, int y) {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x + (i * 3), y); //distributes the Deck horizontally from a specified position
	}
}

string Deck::blackJackValue(string display) { //calculates a hand's value in a game of blackjack
	int value = 0;
	int aces = 0;
	int unknowns = 0;
	string returnVal = "";
	for (int i = 0; i < this->cards.size(); i++) {
		if (this->cards[i].getFace()) { //only face up cards
			if (this->cards[i].getValue() == "A") {
				value += 11;
				aces++;
			}
			else if (this->cards[i].getValue() == "2") {
				value += 2;
			}
			else if (this->cards[i].getValue() == "3") {
				value += 3;
			}
			else if (this->cards[i].getValue() == "4") {
				value += 4;
			}
			else if (this->cards[i].getValue() == "5") {
				value += 5;
			}
			else if (this->cards[i].getValue() == "6") {
				value += 6;
			}
			else if (this->cards[i].getValue() == "7") {
				value += 7;
			}
			else if (this->cards[i].getValue() == "8") {
				value += 8;
			}
			else if (this->cards[i].getValue() == "9") {
				value += 9;
			}
			else if (this->cards[i].getValue() == "X" || this->cards[i].getValue() == "J" || this->cards[i].getValue() == "Q" || this->cards[i].getValue() == "K") {
				value += 10;
			}
		}
		else {
			unknowns++;
		}
	}
	while (value > 21 && aces > 0) {
		value -= 10;
		aces -= 1;
	}

	//converts into a string to show only what the player knows about the hand
	for (int i = 0; i < unknowns; i++) {
		returnVal += "?";
		if (i == unknowns - 1) {
			returnVal += " + ";
		}
	}
	returnVal += to_string(value);

	return returnVal;
}

int Deck::blackJackValue(int raw) { //calculates a hand's value in a game of blackjack
	int value = 0;
	int aces = 0;
	for (int i = 0; i < this->cards.size(); i++) {
		if (this->cards[i].getValue() == "A") {
			value += 11;
			aces++;
		}
		else if (this->cards[i].getValue() == "2") {
			value += 2;
		}
		else if (this->cards[i].getValue() == "3") {
			value += 3;
		}
		else if (this->cards[i].getValue() == "4") {
			value += 4;
		}
		else if (this->cards[i].getValue() == "5") {
			value += 5;
		}
		else if (this->cards[i].getValue() == "6") {
			value += 6;
		}
		else if (this->cards[i].getValue() == "7") {
			value += 7;
		}
		else if (this->cards[i].getValue() == "8") {
			value += 8;
		}
		else if (this->cards[i].getValue() == "9") {
			value += 9;
		}
		else if (this->cards[i].getValue() == "X" || this->cards[i].getValue() == "J" || this->cards[i].getValue() == "Q" || this->cards[i].getValue() == "K") {
			value += 10;
		}
	}
	while (value > 21 && aces > 0) {
		value -= 10;
		aces -= 1;
	}
	return value;
}

void Deck::renderAll() {
	for (int i = 0; i < this->getCards().size(); i++) { //get every Card in the Deck
		this->getCards()[i].render(); //and render it
	}
}