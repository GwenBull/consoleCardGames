#include "GlobalFunctions.h"
#include "Button.h"
using namespace gf;

#pragma once
class UserInterface
{
private:
	vector<vector<Button>> buttons;
	vector<int> selected;

public:
	UserInterface(vector<vector<Button>> buttons);
};

