#include "Button.h"
using namespace std;
using namespace gf;

Button::Button(int x, int y, string text, int function) {
	this->x = x;
	this->y = y;
	this->text = text;
	this->function = function;
}

void Button::render() {
	coords(this->x, this->y + 1);
	cout << "{" << this->text << "}"; //writes the buttons text inside curly braces
}

int Button::getX() {
	return this->x;
}

int Button::getY() {
	return this->y;
}

string Button::getText() {
	return this->text;
}

int Button::getFunctionID() {
	return this->function;
}