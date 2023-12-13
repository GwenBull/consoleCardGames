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
    //extern deck standardDeck;
    static void coords(int x, int y);
    static void clearScreen();
}

void gf::coords(int x, int y) {
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = x;
    coord.Y = y;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(hConsole, coord))
    {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
    }
}

void gf::clearScreen() {
    //std::cout << "\x1B[2J\x1B[H\b";
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    //SetConsoleCursorPosition(console, topLeft);
}