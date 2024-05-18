#include "Card.h"
#include "GlobalFunctions.h"
using namespace std;
using namespace gf;

Card::Card() {
	this->xPos = 0;
	this->yPos = 0;
	this->isFaceUp = false;
} //default constructor

Card::Card(string suit, string value, string colour, int xPos, int yPos) {
	this->suit = suit;
	this->value = value;
	this->colour = colour;
	this->xPos = xPos;
	this->yPos = yPos;
	this->isFaceUp = true;
}

string Card::getSuit() {
	return this->suit;
}

string Card::getValue() {
	return this->value;
}

string Card::getColour() {
	return this->colour;
}

int Card::getX() {
	return this->xPos;
}

int Card::getY() {
	return this->yPos;
}

bool Card::getFace() {
	return this->isFaceUp;
}

void Card::setPos(int newX, int newY) {
	this->xPos = newX; //sets the x and y position of the card
	this->yPos = newY;
}

void Card::flip() {
	this->isFaceUp = !this->isFaceUp; //flips this card
}

void Card::setFace(bool faceUp) {
	this->isFaceUp = faceUp;
}

void Card::render() {
	if (this->isFaceUp) { //if the card is face up
		rect(this->xPos, this->yPos, 5, 4, this->colour);
		coords(this->xPos, this->yPos);
		SetConsoleTextAttribute(hConsole, colours[this->colour]); //sets colour to this cards colour
		gf::coords(this->xPos, this->yPos); //moves the cursor to the top left of this card
		cout << this->value; //draws the top left symbol
		gf::coords(this->xPos + 2, this->yPos + 1); //
		cout << suits[this->suit];
		gf::coords(this->xPos + 4, this->yPos + 3);
		cout << this->value;
	}
	else {
		SetConsoleTextAttribute(hConsole, colours["blue"]);
		gf::coords(this->xPos, this->yPos); //draws a typical blue cardback pattern, line by line
		cout << "\u00C9\u00CD\u00CD\u00CD\u00BB";
		gf::coords(this->xPos, this->yPos + 1);
		cout << "\u00BA\u00C9\u00CD\u00BB\u00BA";
		gf::coords(this->xPos, this->yPos + 2);
		cout << "\u00BA\u00C8\u00CD\u00BC\u00BA";
		gf::coords(this->xPos, this->yPos + 3);
		cout << "\u00C8\u00CD\u00CD\u00CD\u00BC";
	}
}

