#include "GlobalFunctions.h"
using namespace std;

map<string, char> gf::suits = { //maps the name of each card suit to it's unicode symbol
	{"hearts", '\u0003'}, 
	{"diamonds", '\u0004'},
	{"clubs", '\u0005'},
	{"spades", '\u0006'}
};
map<string, int> gf::values = { //maps the face value of each card to it's numerical value
	{"A", 1},
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{"5", 5},
	{"6", 6},
	{"7", 7},
	{"8", 8},
	{"9", 9},
	{"X", 10},
	{"J", 10},
	{"Q", 10},
	{"K", 10} 
};
map<string, int> gf::solitaireValues = { //maps the face value of each card to it's numerical value for solitaire
	{"A", 1},
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{"5", 5},
	{"6", 6},
	{"7", 7},
	{"8", 8},
	{"9", 9},
	{"X", 10},
	{"J", 11},
	{"Q", 12},
	{"K", 13}
};
map<string, int> gf::buttonFuncs = { //maps the names of button functions to an integer value for the function switch statement
	{"null", 0},
	{"toBlackJack", 1},
	{"toSolitaire", 2},
	{"toMenu", 3},
	{"quit", 4},
	{"hit", 5},
	{"stand", 6},
	{"split", 7},
	{"solitaireDraw", 8},
	{"takeFromDeck", 9},
	{"takeFromStack", 10},
	{"stackTakeOne", 11},
	{"stackTakeAll", 12},
	{"undo", 13},
	{"placeOnStack", 14},
	{"placeOnSorted", 15},
	{"undoDraw", 16},
	{"1Deck", 17},
	{"2Deck", 18},
	{"4Deck", 19},
	{"solo", 20},
	{"group", 21},
	{"beginBlackJack", 22}, //ironic
	{"easy", 23},
	{"hard", 24},
	{"beginSolitaire", 25},
	{"takeFromSort", 26}
};
map<string, int> gf::colours = { //maps names of colours to consoleTextAttribute codes
	{"red", 244}, //red text on a white background
	{"black", 240}, //black text on a white background
	{"blue", 243}, //blue text on a white background
	{"whiteText", 15}, //white text on a black background
	{"dGrey", 120}, //dark grey on light grey bg
	{"lGrey", 135} //light grey on dark grey bg 
};
HANDLE gf::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND gf::console = GetConsoleWindow();
RECT gf::r;
int gf::gamemode = 0;