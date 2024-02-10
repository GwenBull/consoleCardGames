#include "UserInterface.h"
#include "GlobalFunctions.h"
using namespace gf;

UserInterface::UserInterface() {
	this->buttons = { {}, {} };
	this->selected = { 0, 0 };
}

UserInterface::UserInterface(vector<vector<Button>> buttons) {
	this->buttons = buttons;
	this->selected = { 0, 0 }; //XY position of current button within the "buttons" 2d vector
}

void UserInterface::renderUI() {
	SetConsoleTextAttribute(hConsole, colours["whiteText"]);
	for (int i = 0; i < buttons.size(); i++) {
		for (int j = 0; j < buttons[i].size(); j++) {
			buttons[i][j].render();
			if (j == this->selected[0] && i == this->selected[1]) {
				this->highlightSelected();
			}
		}
	}
}

void UserInterface::highlightSelected() {
	Button selectedButton = this->buttons[selected[1]][selected[0]];
	vector<int> position = { selectedButton.getX(), selectedButton.getY() };
	coords(position[0], position[1]);
	cout << "\u00C9";
	for (int i = 0; i < selectedButton.getText().length(); i++){
		cout << "\u00CD";
	}
	cout << "\u00BB";
	coords(position[0], position[1] + 1);
	cout << "\u00BA" << selectedButton.getText() << "\u00BA";
	coords(position[0], position[1] + 2);
	cout << "\u00C8";
	for (int i = 0; i < selectedButton.getText().length(); i++) {
		cout << "\u00CD";
	}
	cout << "\u00BC"; //draws a buttons text surrounded 
}

void UserInterface::selectionMoveUp() {
	if (this->selected[1] > 0) { //if a button is available in the up direction
		this->selected[1] -= 1; //move the selection up by one
	}
	if (this->selected[0] >= this->buttons[this->selected[1]].size()) { //If the new selection position is OOB on the other axis
		this->selected[0] = this->buttons[this->selected[1]].size() - 1; //bring it back in
	}
}

void UserInterface::selectionMoveRight() {
	if (this->selected[0] < this->buttons[this->selected[1]].size() - 1) {
		this->selected[0] += 1;
	}
}

void UserInterface::selectionMoveDown() {
	if (this->selected[1] < this->buttons.size() - 1) {
		this->selected[1] += 1;
	}
	if (this->selected[0] >= this->buttons[this->selected[1]].size()) { //If the new selection position is OOB on the other axis
		this->selected[0] = this->buttons[this->selected[1]].size() - 1; //bring it back in
	}
}

void UserInterface::selectionMoveLeft() {
	if (this->selected[0] > 0) {
		this->selected[0] -= 1;
	}
}

Button UserInterface::getCurrentlySelected() {
	return buttons[selected[1]][selected[0]];
}

void UserInterface::copyButtons(UserInterface* buttonSet) {
	this->buttons = buttonSet->buttons;
	if (this->selected[1] >= this->buttons.size()) {
		this->selected[1] = this->buttons.size() - 1;
	}
	if (this->selected[0] >= this->buttons[this->selected[1]].size()) {
		this->selected[0] = this->buttons[this->selected[1]].size() - 1;
	}
}