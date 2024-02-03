#include "globalFunctions.h"
#include <string>
using namespace std;
#pragma once
class button
{
private:
	int x;
	int y;
	string text;
	int function;

public:
	
	button(int x, int y, string text, int function);

	void draw();
};

