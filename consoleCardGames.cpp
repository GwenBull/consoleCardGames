#include "globalFunctions.h"
#include "card.h"
#include "deck.h"
using namespace std;
using namespace gf;

vector<card> deck = {
	card("spades", "A", "black", 0, 0), card("diamonds", "A", "red", 1, 0), card("clubs", "A", "black", 2, 0), card("hearts", "A", "red", 3, 0),
	card("hearts", "A", "red", 4, 0), card("hearts", "A", "red", 5, 0), card("hearts", "A", "black", 6, 0), card("hearts", "A", "red", 7, 0), card("hearts", "A", "red", 8, 0),
	card("hearts", "A", "red", 9, 0), card("clubs", "A", "red", 10, 0)
};
HWND console = GetConsoleWindow();
RECT r;

int main() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;                   // Width of each character in the font
	cfi.dwFontSize.Y = 16;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Lucida Console"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1283, 727, TRUE); //makes the window 720p (the drawable dimensions are 154 by 40 ish)
														 //Numbers slightly off because things I don't understand make it not exactly the written numbers
		//for (int i = 0; i < 256; i++) {
	//	SetConsoleTextAttribute(hConsole, i);
	//	char chari = i;
	//	cout << i << ": " << chari << "\n";
	//}
	deck[0].flip();
	while (true){
		for (int k = 0; k < deck.size(); k++) {
			unsigned int seed = static_cast<unsigned int>(time(NULL));
			srand(seed);
			int r = k + rand() % (deck.size() - k); // careful here!
			swap(deck[k], deck[r]);
		}
		for (int i = 0; i < deck.size(); i++) {
			deck[i].setPos(i * 3, deck[i].getY());
		}
		for (int i = 0; i < 11; i++) {
			deck[i].render();
		}
		if (_kbhit()) {
			int ch = _getch();
			if (ch == 224) {
				ch = _getch();
			}
			if (ch == 77) {
				deck[0].setPos(deck[0].getX() + 1, deck[0].getY());
			}
			else if (ch == 80){
				deck[0].setPos(deck[0].getX(), deck[0].getY() + 1);
			}
			else if(ch == 72){
				deck[0].setPos(deck[0].getX(), deck[0].getY() - 1);
			}
			else if (ch == 75){
				deck[0].setPos(deck[0].getX() - 1, deck[0].getY());
			}
			else if (ch == 82 || ch == 114) {
				deck[0].flip();
			}
		}
		Sleep(83);
		clearScreen();
	}
		//while (true) {
		//	int ch = _getch();
		//	cout << ch;
		//	Sleep(1000);
		//}
}