#include "UserInterface.h"

UserInterface::UserInterface(vector<vector<Button>> buttons) {
	this->buttons = buttons;
	this->selected = { 0, 0 };
}

