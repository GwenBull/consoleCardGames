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
	cout << "{" << this->text << "}";
}