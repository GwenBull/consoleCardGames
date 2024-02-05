#include "GlobalFunctions.h"
using namespace std;
#pragma once

class Card
{
private:
	string house;
	string value;
	int xPos;
	int yPos;
	string colour;
	bool isFaceUp;

public:

	Card();

	Card(string house, string value, string colour, int xPos, int yPos);

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
