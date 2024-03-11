#include "GlobalFunctions.h"
using namespace std;

map<string, char> gf::suits = { {"hearts", '\u0003'}, {"diamonds", '\u0004'} ,{"clubs", '\u0005'}, {"spades", '\u0006'} };
map<string, int> gf::values = { {"A", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8 }, {"9", 9}, {"X", 10}, {"J", 10}, {"Q", 10}, {"K", 10} };
HANDLE gf::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
map<string, int> gf::colours = { {"red", 244}, {"black", 240}, {"blue", 243}, {"whiteText", 15}, {"dGrey", 120}, {"lGrey", 135} };
HWND gf::console = GetConsoleWindow();
RECT gf::r;
int gf::gamemode = 0;