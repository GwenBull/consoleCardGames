#include "deck.h"
#include "globalFunctions.h"
using namespace std;

deck::deck(vector<card> cards) {
	this->cards = cards;
}

vector<card> deck::getCards() { 
	return this->cards; //returns the entire set of cards in this deck/hand
}

card deck::drawTopCard() {
	card drawnCard = this->cards[this->cards.size() - 1]; //copies the back card
	this->cards.pop_back(); //removes the back card from the deck
	return drawnCard; //returns the card that has been removed
	//because card rendering iterates forwards through each deck,
	//the last indexed item is the one drawn at the top, so I am
	//treating it as such.
	//basically: the "top" of the deck, is the "back" of the vector.
	//Wait that's just a stack :p
}

void deck::placeCardAtTop(card newCard) {
	this->cards.push_back(newCard); //puts a new card at the top of the deck
}

void deck::shuffle() {
	for (int i = 0; i < this->cards.size(); i++) { //iterates through the deck
		unsigned int seed = static_cast<unsigned int>(time(NULL)); //generates a seed (current time as int)
		srand(seed); //puts that seed into the built in random function
		int r = i + rand() % (this->cards.size() - i); //selects a random card in the deck
		swap(this->cards[i], this->cards[r]); //swaps card at current iteration with
	}
}

void deck::showAll() {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].isFaceUp = true; //sets every card in this deck to face up
	}
}

void deck::hideAll() {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].isFaceUp = false; //sets every card in this deck to face down
	}
}

void deck::stack(int x, int y) {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y); //places this entire deck at the specified position
	}
}

void deck::spreadVert(int x, int y) {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y + (i * 2)); //distributes the deck vertically from a specified position
	}
}

void deck::spreadHoriz(int x, int y) {
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x + (i * 3), y); //distributes the deck horizontally from a specified position
	}
}

int deck::blackJackValue() { //calculates a hand's value in a game of blackjack
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