#include "card.h"
#include "globalFunctions.h"
using namespace std;
using namespace gf;

card::card(string house, string value, string colour, int xPos, int yPos) {
	this->house = house;
	this->value = value;
	this->colour = colour;
	this->xPos = xPos;
	this->yPos = yPos;
	this->isFaceUp = true;
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
	if (this->isFaceUp) {
		SetConsoleTextAttribute(hConsole, colours[this->colour]);
		gf::coords(this->xPos, this->yPos);
		cout << this->value << "    ";
		gf::coords(this->xPos, this->yPos + 1);
		cout << "  " << suits[this->house] << "  ";
		gf::coords(this->xPos, this->yPos + 2);
		cout << "     ";
		gf::coords(this->xPos, this->yPos + 3);
		cout << "    " << this->value;
	}
	else {
		SetConsoleTextAttribute(hConsole, colours["blue"]);
		gf::coords(this->xPos, this->yPos);
		cout << "\u00C9\u00CD\u00CD\u00CD\u00BB";
		gf::coords(this->xPos, this->yPos + 1);
		cout << "\u00BA\u00C9\u00CD\u00BB\u00BA";
		gf::coords(this->xPos, this->yPos + 2);
		cout << "\u00BA\u00C8\u00CD\u00BC\u00BA";
		gf::coords(this->xPos, this->yPos + 3);
		cout << "\u00C8\u00CD\u00CD\u00CD\u00BC";
	}
}