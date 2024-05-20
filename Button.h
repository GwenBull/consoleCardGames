#include "GlobalFunctions.h"
#include <string>
using namespace std;
#pragma once
class Button
{
private:
	int x;
	int y;
	string text;
	string function;

public:
	
	Button(int x, int y, string text, string function);

	void render();

	int getX();

	int getY();

	string getText();

	int getFunctionID();
};

