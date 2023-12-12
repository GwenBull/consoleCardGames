#include <iostream>
#include <string>
#include "card.h"
#include "globalFunctions.h"
using namespace std;

card::card(string house, string value, string colour, int xPos, int yPos) {
	this->house = house;
	this->value = value;
	this->colour = colour;
	this->xPos = xPos;
	this->yPos = yPos;
	this->isFaceUp = false;
}

string card::getHouse() {
	return this->house;
}

string card::getValue() {
	return this->value;
}

int card::getX() {
	return this->xPos;
}

int card::getY() {
	return this->yPos;
}

void card::setPos(int newX, int newY) {
	this->xPos = newX;
	this->yPos = newY;
}

void card::flip() {
	this->isFaceUp = !this->isFaceUp;
}

void card::render() {
	gf::coords(this->xPos, this->yPos);
	cout << '\u00C9' << '\u00CD' << '\u00CD' << '\u00CD' << '\u00BB';
	gf::coords(this->xPos, this->yPos + 1);
	cout << '\u00BA' << this->value << "  " << '\u00BA';
	gf::coords(this->xPos, this->yPos + 2);
	cout << '\u00BA' << " " << gf::suits[this->house] << " " << '\u00BA';
	gf::coords(this->xPos, this->yPos + 3);
	cout << '\u00BA' << "  " << this->value << '\u00BA';
	gf::coords(this->xPos, this->yPos + 4);
	cout << '\u00C8' << '\u00CD' << '\u00CD' << '\u00CD' << '\u00BC';
}