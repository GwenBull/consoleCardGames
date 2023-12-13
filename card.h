#include "globalFunctions.h"
using namespace std;
#pragma once

class card
{
private:
	string house;
	string value;
	int xPos;
	int yPos;

public:
	string colour;
	bool isFaceUp;

	card(string house, string value, string colour, int xPos, int yPos);

	string getHouse();

	string getValue();

	int getX();

	int getY();

	void setPos(int newX, int newY);

	void flip();

	void render();
};
