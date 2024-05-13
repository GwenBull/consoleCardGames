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

void UserInterface::renderUI() { //draws every button in the current UI
	SetConsoleTextAttribute(hConsole, colours["whiteText"]);
	for (int i = 0; i < buttons.size(); i++) { //for every button row
		for (int j = 0; j < buttons[i].size(); j++) { //for every button per row
			buttons[i][j].render(); //draw the button
			if (j == this->selected[0] && i == this->selected[1]) { //if it's the selected button
				this->highlightSelected(); //draw the highlight
			}
		}
	}
}

void UserInterface::highlightSelected() { //draws a border around the currently selected button
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
	cout << "\u00BC";
}

void UserInterface::selectionMoveUp() { //moves the button cursor upwards
	int prevDispX = buttons[this->selected[1]][this->selected[0]].getX(); //gets the display location of the button highlighted before movement is appllied
	if (this->selected[1] > 0) { //if a button is available in the up direction
		this->selected[1] -= 1; //move the selection up by one
	}
	vector<int> newRowDispXs; //stores the X position of every Button on the newly selected row
	for (int i = 0; i < buttons[this->selected[1]].size(); i++) {
		newRowDispXs.push_back(buttons[this->selected[1]][i].getX());
	}
	int newButtonX = findMinDiff(prevDispX, newRowDispXs);
	this->selected[0] = newButtonX; //moves the cursor horizontally to the one nearest to the previous selection
}

void UserInterface::selectionMoveRight() { //moves button cursor rightwards
	if (this->selected[0] < this->buttons[this->selected[1]].size() - 1) { //if there's a button to the right
		this->selected[0] += 1;
	}
}

void UserInterface::selectionMoveDown() { //move button cursor downwards
	int prevDispX = buttons[this->selected[1]][this->selected[0]].getX(); //gets the display location of the button highlighted before movement is appllied
	if (this->selected[1] < this->buttons.size() - 1) { //if another row exists below
		this->selected[1] += 1;
	}
	vector<int> newRowDispXs; //stores the X position of every Button on the newly selected row
	for (int i = 0; i < buttons[this->selected[1]].size(); i++) {
		newRowDispXs.push_back(buttons[this->selected[1]][i].getX());
	}
	int newButtonX = findMinDiff(prevDispX, newRowDispXs);
	this->selected[0] = newButtonX; //moves the cursor horizontally to the one nearest to the previous selection
}

void UserInterface::selectionMoveLeft() { //move button cursor leftwards
	if (this->selected[0] > 0) { //if a button is available to the left
		this->selected[0] -= 1; //move cursor left by one
	}
}

Button UserInterface::getCurrentlySelected() {
	return buttons[selected[1]][selected[0]];
}

vector<int> UserInterface::getSelectionVal() {
	return this->selected;
}

void UserInterface::copyButtons(UserInterface* buttonSet) { //takes the buttons from another ui and copies them into this
	this->buttons = buttonSet->buttons; //clones the buttons
	if (this->selected[1] >= this->buttons.size()) { //checks if the cursor is now OOB
		this->selected[1] = this->buttons.size() - 1; //and brings it back in if necessary
	}
	if (this->selected[0] >= this->buttons[this->selected[1]].size()) { // does the same on the other axis
		this->selected[0] = this->buttons[this->selected[1]].size() - 1;
	}
}