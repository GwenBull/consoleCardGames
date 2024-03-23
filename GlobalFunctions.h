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
    //global variables (defined in GlobalFunctions.cpp)
    extern map<string, char> suits;
    extern map<string, int> values;
    extern map<string, int> solitaireValues;
    extern HANDLE hConsole;
    extern map<string, int> colours;
    extern HWND console;
    extern RECT r;
    extern int gamemode;
    extern map<string, int> buttonFuncs;

    //global functions (defined below)
    static void coords(int x, int y);
    static void clearScreen();
    static void drawBigSpade();
    static void rect(int x, int y, int w, int h, string colour);
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

void gf::drawBigSpade() { //draws a big spade logo, for the main menu
    SetConsoleTextAttribute(hConsole, colours["black"]);
    coords(70, 2);
    cout << "    ";
    coords(70, 3);
    cout << "    ";
    coords(68, 4);
    cout << "        ";
    coords(66, 5);
    cout << "            ";
    coords(66, 6);
    cout << "            ";
    coords(64, 7);
    cout << "                ";
    coords(62, 8);
    cout << "                    ";
    coords(60, 9);
    cout << "                        ";
    coords(58, 10);
    cout << "                            ";
    coords(56, 11);
    cout << "                                ";
    coords(52, 12);
    cout << "                                        ";
    coords(50, 13);
    cout << "                                            ";
    coords(48, 14);
    cout << "                                                ";
    coords(48, 15);
    cout << "                                                ";
    coords(46, 16);
    cout << "                                                    ";
    coords(46, 17);
    cout << "                                                    ";
    coords(46, 18);
    cout << "                                                    ";
    coords(46, 19);
    cout << "                                                    ";
    coords(46, 20);
    cout << "                                                    ";
    coords(48, 21);
    cout << "                                                ";
    coords(48, 22);
    cout << "                                                ";
    coords(50, 23);
    cout << "                                            ";
    coords(54, 24);
    cout << "                                    ";
    coords(56, 25);
    cout << "      ";
    coords(68, 25);
    cout << "        ";
    coords(82, 25);
    cout << "      ";
    coords(66, 26);
    cout << "            ";
    coords(66, 27);
    cout << "            ";
    coords(66, 28);
    cout << "            ";
    coords(64, 29);
    cout << "                ";
    coords(60, 30);
    cout << "                        ";
    coords(60, 31);
    cout << "                        ";
    SetConsoleTextAttribute(hConsole, colours["whiteText"]);
}

void gf::rect(int x, int y, int w, int h, string colour) {
    SetConsoleTextAttribute(hConsole, colours[colour]);
    for (int i = 0; i < h; i++) {
        coords(x, y + i);
        for (int j = 0; j < w; j++) {
            cout << " ";
        }
    }
}