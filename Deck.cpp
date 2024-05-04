#include "Deck.h"
#include "GlobalFunctions.h"
using namespace std;

Deck::Deck() {
	this->cards = {};
}

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

void Deck::hideAll() { //sets every Card in this Deck to face down
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setFace(false);
	}
}

void Deck::flipSpecific(size_t which, bool face) { //uses size_t instead of int because vector.size() is a size_t, so the function uses it to prevent a compiler warning about truncation
												   //which happens when casting size_t to int
	this->cards[which].setFace(face);
}

void Deck::stack(int x, int y) { //places this entire Deck at the specified position
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y);
	}
}

void Deck::spreadVert(int x, int y) { //distributes the Deck vertically from a specified position
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y + (i * 2));
	}
}

void Deck::spreadVertCompressed(int x, int y) { //distributes the Deck vertically from a specified position with a smaller gap
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y + i);
	}
}

void Deck::spreadVertLimited(int x, int y, int max) { //distributes the top n cards of the Deck vertically from a specified position
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x, y);
	}
	for (int i = 0; i < max; i++) {
		this->cards[this->cards.size() - (max - i)].setPos(x, y + (i * 2));
	}
}

void Deck::spreadHoriz(int x, int y) { //distributes the Deck horizontally from a specified position
	for (int i = 0; i < this->cards.size(); i++) {
		this->cards[i].setPos(x + (i * 3), y);
	}
}

string Deck::blackJackValue(string display) { //calculates a hand's value in a game of blackjack and returns it with some visual data attached
											  //simplifies the function calls that need to be made when displaying opponents to the player
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

int Deck::blackJackValue(int display) { //calculates a hand's value in a game of blackjack
	int value = 0;
	int aces = 0;
	//It has to do a load of ifs because card.value is a string, and they need to be added as ints, not concatenated
	//And they have to be strings because some have a letter as their value
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

void Deck::reverseCards() {
	reverse(this->cards.begin(), this->cards.end());

}

void Deck::copyAll(Deck* otherDeck) { //moves a copy of each card in otherDeck to the deck calling the function (without removing them from otherDeck
	int size = otherDeck->getCards().size();
	for (int i = 0; i < size; i++) {
		Card drawnCard = otherDeck->drawTopCard();
		this->placeCardAtTop(drawnCard);
		otherDeck->reverseCards();
		otherDeck->placeCardAtTop(drawnCard);
		otherDeck->reverseCards();
	}
}

void Deck::empty() {
	for (int i = 0; i < this->getCards().size(); i++) {
		this->drawTopCard();
	}
}

void Deck::moveCard(int index, int newX, int newY) {
	this->cards[index].setPos(newX, newY);
}