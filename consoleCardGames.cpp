#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "card.h"
#include "globalFunctions.h"
using namespace std;
using namespace gf;

vector<card> deck = { card("spades", "A", "black", 16, 0), card("spades", "2", "black", 1, 2), card("spades", "3", "black", 3, 1) };
HWND console = GetConsoleWindow();
RECT r;

int main() {
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1280, 720, TRUE); //makes the window 720p
	while (true){
		deck[0].render();
		deck[1].render();
		deck[2].render();
		_getch();
		clearScreen();
		deck[0].setPos(deck[0].getX() + 1, deck[0].getY());
	}
}