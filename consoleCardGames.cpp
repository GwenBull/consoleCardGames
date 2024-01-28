#include "globalFunctions.h"
#include "card.h"
#include "deck.h"
using namespace std;
using namespace gf;

HWND console = GetConsoleWindow();
RECT r;

void init() {
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

	for (int i = 0; i < 256; i++) {
		SetConsoleTextAttribute(hConsole, i);
		char chari = i;
		cout << i << ": " << chari << "\n";
	}
	clearScreen();
	coords(0, 0);
	SetConsoleTextAttribute(hConsole, 7);
	cout << "Game ready, press any key to continue";
	_getch();
	clearScreen();
}

int main() {
	init();

	deck standardDeck = deck(
		vector<card>{
			card("diamonds", "A", "red", 6, 0), card("diamonds", "2", "red", 0, 0), card("diamonds", "3", "red", 0, 0), card("diamonds", "4", "red", 0, 0),
			card("diamonds", "5", "red", 0, 0), card("diamonds", "6", "red", 0, 0), card("diamonds", "7", "red", 0, 0), card("diamonds", "8", "red", 0, 0), 
			card("diamonds", "9", "red", 0, 0), card("diamonds", "X", "red", 0, 0), card("diamonds", "J", "red", 0, 0), card("diamonds", "Q", "red", 0, 0), 
			card("diamonds", "K", "red", 0, 0),
			card("clubs", "A", "black", 0, 0), card("clubs", "2", "black", 0, 8), card("clubs", "3", "black", 0, 0), card("clubs", "4", "black", 0, 0),
			card("clubs", "5", "black", 0, 0), card("clubs", "6", "black", 0, 0), card("clubs", "7", "black", 0, 0), card("clubs", "8", "black", 0, 0), 
			card("clubs", "9", "black", 0, 0), card("clubs", "X", "black", 0, 0), card("clubs", "J", "black", 0, 0), card("clubs", "Q", "black", 0, 0), 
			card("clubs", "K", "black", 0, 0),
			card("hearts", "A", "red", 0, 0), card("hearts", "2", "red", 0, 0), card("hearts", "3", "red", 0, 0), card("hearts", "4", "red", 0, 0),
			card("hearts", "5", "red", 0, 0), card("hearts", "6", "red", 4, 5), card("hearts", "7", "red", 0, 0), card("hearts", "8", "red", 0, 0), 
			card("hearts", "9", "red", 0, 0), card("hearts", "X", "red", 0, 0), card("hearts", "J", "red", 0, 0), card("hearts", "Q", "red", 0, 0), 
			card("hearts", "K", "red", 0, 0),
			card("spades", "A", "black", 0, 0), card("spades", "2", "black", 0, 0), card("spades", "3", "black", 0, 0), card("spades", "4", "black", 0, 0), 
			card("spades", "5", "black", 0, 0), card("spades", "6", "black", 0, 0), card("spades", "7", "black", 0, 0), card("spades", "8", "black", 0, 0), 
			card("spades", "9", "black", 0, 0), card("spades", "X", "black", 0, 0), card("spades", "J", "black", 0, 0), card("spades", "Q", "black", 0, 0), 
			card("spades", "K", "black", 0, 0)
	});
	deck playerHand = deck(vector<card>{});
	deck dealerHand = deck(vector<card>{});
	standardDeck.shuffle();

	standardDeck.hideAll();
	standardDeck.stack(0, 0);

	while (true){ //main game loop for blackjack
		for (int i = 0; i < standardDeck.getCards().size(); i++) { //get every card in the deck
			standardDeck.getCards()[i].render(); //and render it
		}
		playerHand.spreadHoriz(10, 10); //draws the players cards spread from position 10, 10
		for (int i = 0; i < playerHand.getCards().size(); i++) {
			playerHand.getCards()[i].render();
		}
		gf::coords(10, 16);
		cout << "Total: " << playerHand.blackJackValue();
		int ch = _getch(); //gets the latest keyboard input
		if (ch == 224) { //since arrow keys technically give 2 inputs, this prunes the first, useless one
			ch = _getch(); //then gets the real arrow value
		}
		switch (ch) {
		case 72: //up arrow (also capital H)
			break;
		case 80: //down arrow (also capital P)
			break;
		case 75: //left arrow (also capital K)
			break;
		case 77: //right arrow (also capital M)
			break;
		case 13: //enter
			playerHand.placeCardAtTop(standardDeck.drawTopCard());
			playerHand.showAll();
		default:
			break;
		}
		//Sleep(100);
		clearScreen();
	}
		//while (true) {
		//	int ch = _getch();
		//	cout << ch;
		//	Sleep(1000);
		//}
}