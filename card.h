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
	string colour;
	bool isFaceUp;

public:

	card();

	card(string house, string value, string colour, int xPos, int yPos);

	string getHouse();

	string getValue();

	int getX();

	int getY();

	bool getFace();

	void setPos(int newX, int newY);

	void flip();

	void setFace(bool faceUp);

	void render();
};
