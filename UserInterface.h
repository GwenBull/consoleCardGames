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
	UserInterface();

	UserInterface(vector<vector<Button>> buttons);

	void renderUI();

	void highlightSelected();

	void selectionMoveUp();

	void selectionMoveRight();

	void selectionMoveDown();

	void selectionMoveLeft();

	Button getCurrentlySelected();

	vector<int> getSelectionVal();

	void copyButtons(UserInterface* buttonSet);
};

