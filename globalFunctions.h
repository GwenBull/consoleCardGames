#include <iostream>
#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
using namespace std;
#pragma once

namespace gf {
    extern map<string, char> suits;
    extern HANDLE hConsole;
    extern map<string, int> colours;
    static void coords(int x, int y);
    static void clearScreen();
}

void gf::coords(int x, int y) {
    COORD coord; //create a COORD structure
    coord.X = x; //set the x and y coord
    coord.Y = y;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //gets the console

    SetConsoleCursorPosition(hConsole, coord); //puts the cursor at the specified position in the console
}

void gf::clearScreen() {
    COORD topLeft = { 0, 0 }; //defines the starting position of the console
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //gets the console
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen); //gets information about the screen size
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    ); //fills every position on the screen with blank spaces
    //if I wanted it to, this could fill the screen with any character
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    ); //resets the text attributes of the coneole
    //SetConsoleCursorPosition(console, topLeft);
}