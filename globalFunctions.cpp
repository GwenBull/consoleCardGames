#include "globalFunctions.h"
using namespace std;

map<string, char> gf::suits = { {"hearts", '\u0003'}, {"diamonds", '\u0004'} ,{"clubs", '\u0005'}, {"spades", '\u0006'} };
HANDLE gf::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
map<string, int> gf::colours = { {"red", 116}, {"black", 112}, {"blue", 121} };