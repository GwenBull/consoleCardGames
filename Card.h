#include "GlobalFunctions.h"
using namespace std;
#pragma once

class Card
{
private:
	string suit;
	string value;
	int xPos;
	int yPos;
	string colour;
	bool isFaceUp;

public:

	Card();

	Card(string suit, string value, string colour, int xPos, int yPos);

	string getSuit();

	string getValue();
	
	string getColour();

	int getX();

	int getY();

	bool getFace();

	void setPos(int newX, int newY);

	void flip();

	void setFace(bool faceUp);

	void render();
};
