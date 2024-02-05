#include "GlobalFunctions.h"
#include "Card.h"
#include "Deck.h"
#include "Button.h"
using namespace std;
using namespace gf;

HWND console = GetConsoleWindow();
RECT r;
int whiteText = 7;
int whoseTurn = 0;
vector<vector<int>> talkCoords = { {25, 10}, {50, 20}, {75, 30}, {100, 20}, {125, 10}, {75, 0} };
vector<vector<int>> handCoords = { {25, 11}, {50, 21}, {75, 31}, {100, 21}, {125, 11}, {75, 1} };
vector<Deck> turnOrder;
vector<vector<int>> scoreCoords = { {25, 15}, {50, 25}, {75, 35}, {100, 25}, {125, 15}, {75, 5} };

void init() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;                   // Width of each character in the font
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
	//clearScreen();
	coords(0, 0);
	SetConsoleTextAttribute(hConsole, 7);
	cout << "Game ready, press any key to continue";
	_getch();
	clearScreen();
}

void firstDeal(Deck* mainPile, Deck* dealer, Deck* player, Deck* ai1, Deck* ai2, Deck* ai3, Deck* ai4) {
	Card drawnCard;
	vector<Deck*> dealOrder = { ai1, ai2, player, ai3, ai4, dealer };
	for (int i = 0; i < 2; i++) { //for the 2 initial cards per player
		for (int j = 0; j < 6; j++) { //for each player + dealer
			Sleep(415); //wait a reasonable time
			drawnCard = mainPile->drawTopCard(); //draw a Card
			if (j == 2 || (j == 5 && i == 1)) { //if it's the player's Card or the dealers last Card
				drawnCard.flip(); //make it visible
			}
			dealOrder[j]->placeCardAtTop(drawnCard); //deal the Card
			dealOrder[j]->spreadHoriz(handCoords[j][0], handCoords[j][1]); //put it in the right position
			dealOrder[j]->renderAll(); //render it
		}
	}
}

int main() {
	int currentTotal;
	init();

	Card drawnCard;
	Deck standardDeck = Deck(
		vector<Card>{
			Card("diamonds", "A", "red", 6, 0), Card("diamonds", "2", "red", 0, 0), Card("diamonds", "3", "red", 0, 0), Card("diamonds", "4", "red", 0, 0),
			Card("diamonds", "5", "red", 0, 0), Card("diamonds", "6", "red", 0, 0), Card("diamonds", "7", "red", 0, 0), Card("diamonds", "8", "red", 0, 0), 
			Card("diamonds", "9", "red", 0, 0), Card("diamonds", "X", "red", 0, 0), Card("diamonds", "J", "red", 0, 0), Card("diamonds", "Q", "red", 0, 0), 
			Card("diamonds", "K", "red", 0, 0),
			Card("clubs", "A", "black", 0, 0), Card("clubs", "2", "black", 0, 8), Card("clubs", "3", "black", 0, 0), Card("clubs", "4", "black", 0, 0),
			Card("clubs", "5", "black", 0, 0), Card("clubs", "6", "black", 0, 0), Card("clubs", "7", "black", 0, 0), Card("clubs", "8", "black", 0, 0), 
			Card("clubs", "9", "black", 0, 0), Card("clubs", "X", "black", 0, 0), Card("clubs", "J", "black", 0, 0), Card("clubs", "Q", "black", 0, 0), 
			Card("clubs", "K", "black", 0, 0),
			Card("hearts", "A", "red", 0, 0), Card("hearts", "2", "red", 0, 0), Card("hearts", "3", "red", 0, 0), Card("hearts", "4", "red", 0, 0),
			Card("hearts", "5", "red", 0, 0), Card("hearts", "6", "red", 4, 5), Card("hearts", "7", "red", 0, 0), Card("hearts", "8", "red", 0, 0), 
			Card("hearts", "9", "red", 0, 0), Card("hearts", "X", "red", 0, 0), Card("hearts", "J", "red", 0, 0), Card("hearts", "Q", "red", 0, 0), 
			Card("hearts", "K", "red", 0, 0),
			Card("spades", "A", "black", 0, 0), Card("spades", "2", "black", 0, 0), Card("spades", "3", "black", 0, 0), Card("spades", "4", "black", 0, 0), 
			Card("spades", "5", "black", 0, 0), Card("spades", "6", "black", 0, 0), Card("spades", "7", "black", 0, 0), Card("spades", "8", "black", 0, 0), 
			Card("spades", "9", "black", 0, 0), Card("spades", "X", "black", 0, 0), Card("spades", "J", "black", 0, 0), Card("spades", "Q", "black", 0, 0), 
			Card("spades", "K", "black", 0, 0)
	});
	Deck playerHand = Deck(vector<Card>{});
	Deck dealerHand = Deck(vector<Card>{});
	Deck ai1Hand = Deck(vector<Card>{});
	Deck ai2Hand = Deck(vector<Card>{});
	Deck ai3Hand = Deck(vector<Card>{});
	Deck ai4Hand = Deck(vector<Card>{});

	Button testButton = Button(5, 5, "press me", 0);
	
	turnOrder = { ai1Hand, ai2Hand, playerHand, ai3Hand, ai4Hand, dealerHand };
	
	//preparing the Deck on the table
	standardDeck.shuffle();
	standardDeck.hideAll();
	standardDeck.flipSpecific(0, true);
	standardDeck.stack(1, 1);

	standardDeck.renderAll();
	firstDeal(&standardDeck, &dealerHand, &playerHand, &ai1Hand, &ai2Hand, &ai3Hand, &ai4Hand);

	while (true){ //main game loop for blackjack

		//rendering
			//cards
		standardDeck.renderAll();
		ai1Hand.spreadHoriz(handCoords[0][0], handCoords[0][1]);
		ai1Hand.renderAll();
		ai2Hand.spreadHoriz(handCoords[1][0], handCoords[1][1]);
		ai2Hand.renderAll();
		playerHand.spreadHoriz(handCoords[2][0], handCoords[2][1]); //draws the players cards spread from position 10, 10
		playerHand.renderAll();
		ai3Hand.spreadHoriz(handCoords[3][0], handCoords[3][1]);
		ai3Hand.renderAll();
		ai4Hand.spreadHoriz(handCoords[4][0], handCoords[4][1]);
		ai4Hand.renderAll();
		dealerHand.spreadHoriz(handCoords[5][0], handCoords[5][1]);
		dealerHand.renderAll();
			//show hand values
		SetConsoleTextAttribute(hConsole, whiteText);
		gf::coords(scoreCoords[0][0], scoreCoords[0][1]);
		cout << "Total: " << ai1Hand.blackJackValue("string");
		gf::coords(scoreCoords[1][0], scoreCoords[1][1]);
		cout << "Total: " << ai2Hand.blackJackValue("string");
		gf::coords(scoreCoords[2][0], scoreCoords[2][1]);
		cout << "Total: " << playerHand.blackJackValue("string");
		gf::coords(scoreCoords[3][0], scoreCoords[3][1]);
		cout << "Total: " << ai3Hand.blackJackValue("string");
		gf::coords(scoreCoords[4][0], scoreCoords[4][1]);
		cout << "Total: " << ai4Hand.blackJackValue("string");
		gf::coords(scoreCoords[5][0], scoreCoords[5][1]);
		cout << "Total: " << dealerHand.blackJackValue("string");
			//buttons
		testButton.render();

		switch (whoseTurn) {
		case 0: //first ai
			currentTotal = ai1Hand.blackJackValue(1);
			if (currentTotal < 16) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.flip();
				ai1Hand.placeCardAtTop(drawnCard);
				gf::coords(talkCoords[0][0], talkCoords[0][1]);
				cout << "Hit";
			}
			else if (currentTotal <= 21) {
				gf::coords(talkCoords[0][0], talkCoords[0][1]);
				cout << "Stand";
				whoseTurn++;
			}
			else {
				gf::coords(talkCoords[0][0], talkCoords[0][1]);
				ai1Hand.showAll();
				cout << "Bust";
				whoseTurn++;
			}
			break;
		case 1: //second ai
			currentTotal = ai2Hand.blackJackValue(1);
			if (currentTotal < 21) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.flip();
				ai2Hand.placeCardAtTop(drawnCard);
				gf::coords(talkCoords[1][0], talkCoords[1][1]);
				cout << "Hit";
			}
			else if (currentTotal <= 21) {
				gf::coords(talkCoords[1][0], talkCoords[1][1]);
				cout << "Stand";
				whoseTurn++;
			}
			else {
				gf::coords(talkCoords[1][0], talkCoords[1][1]);
				ai2Hand.showAll();
				cout << "Bust";
				whoseTurn++;
			}
			break;
		case 3: //third ai (after the player)
			currentTotal = ai3Hand.blackJackValue(1);
			if (currentTotal < 17) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.flip();
				ai3Hand.placeCardAtTop(drawnCard);
				gf::coords(talkCoords[3][0], talkCoords[3][1]);
				cout << "Hit";
			}
			else if (currentTotal <= 21) {
				gf::coords(talkCoords[3][0], talkCoords[3][1]);
				cout << "Stand";
				whoseTurn++;
			}
			else {
				gf::coords(talkCoords[3][0], talkCoords[3][1]);
				ai3Hand.showAll();
				cout << "Bust";
				whoseTurn++;
			}
			break;
		case 4: //fourth ai
			currentTotal = ai4Hand.blackJackValue(1);
			if (currentTotal < 17) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.flip();
				ai4Hand.placeCardAtTop(drawnCard);
				gf::coords(talkCoords[4][0], talkCoords[4][1]);
				cout << "Hit";
			}
			else if (currentTotal <= 21) {
				gf::coords(talkCoords[4][0], talkCoords[4][1]);
				cout << "Stand";
				whoseTurn++;
			}
			else {
				gf::coords(talkCoords[4][0], talkCoords[4][1]);
				ai4Hand.showAll();
				cout << "Bust";
				whoseTurn++;
			}
			break;
		case 5: //fifth ai (dealer)
			currentTotal = dealerHand.blackJackValue(1);
			if (currentTotal < 17) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.flip();
				dealerHand.placeCardAtTop(drawnCard);
				gf::coords(talkCoords[5][0], talkCoords[5][1]);
				cout << "Hit";
			}
			else if (currentTotal <= 21) {
				gf::coords(talkCoords[5][0], talkCoords[5][1]);
				cout << "Stand";
				whoseTurn++;
			}
			else {
				gf::coords(talkCoords[5][0], talkCoords[5][1]);
				dealerHand.showAll();
				cout << "Bust";
				whoseTurn++;
			}
			break;
		}
		SetConsoleTextAttribute(hConsole, 0);

		//input processing
		int ch = _getch(); //gets the latest keyboard input
		if (ch == 224) { //since arrow keys technically give 2 inputs, this prunes the first, useless one
			ch = _getch(); //then gets the real arrow value
			switch (ch) { //this switch is nested here so it will only respond to presses of the arrow keys
			case 72: //up arrow
				break;
			case 80: //down arrow
				break;
			case 75: //left arrow
				break;
			case 77: //right arrow
				break;
			default:
				break;
			}
		}
		else { //this switch responds to everything else
			switch (ch) {
			case 13: //enter
				if (standardDeck.getCards().size() > 1 && whoseTurn == 2) {
					playerHand.placeCardAtTop(standardDeck.drawTopCard());
					playerHand.showAll();
				}
				break;
			case 32:
				whoseTurn++;
				break;
			default:
				break;
			}
		}
		clearScreen();
	}
}