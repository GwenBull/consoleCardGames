#include "Card.h"
#include "Deck.h"
#include "UserInterface.h"
using namespace std;
using namespace gf;

//Global variables
//Window control
//
int whiteText = 7;
int whoseTurn = 0;

vector<vector<int>> talkCoords = { {25, 10}, {50, 20}, {75, 30}, {100, 20}, {125, 10}, {75, 0} };
vector<vector<int>> handCoords = { {25, 11}, {50, 21}, {75, 31}, {100, 21}, {125, 11}, {75, 1} };
vector<Deck*> turnOrder;
vector<vector<int>> scoreCoords = { {25, 15}, {50, 25}, {75, 35}, {100, 25}, {125, 15}, {75, 5} };

Card drawnCard;
Deck standardDeck;
Deck playerHand;
Deck dealerHand;
Deck ai1Hand;
Deck ai2Hand;
Deck ai3Hand;
Deck ai4Hand;

Deck resetDeck = Deck( //A full deck of cards used to reset other decks
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

void init() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8; // Width of each character in the font
	cfi.dwFontSize.Y = 16; // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Lucida Console"); //forces a font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1283, 727, TRUE); //makes the window 720p (the drawable dimensions are 154 by 40 ish)
	//Numbers slightly off because things I don't understand make it not exactly the written numbers
	clearScreen();
}

void firstDeal(Deck* mainPile, Deck* dealer, Deck* player, Deck* ai1, Deck* ai2, Deck* ai3, Deck* ai4) {
	Card drawnCard;
	vector<Deck*> dealOrder = { ai1, ai2, player, ai3, ai4, dealer };
	for (int i = 0; i < 2; i++) { //for the 2 initial cards per player
		for (int j = 0; j < 6; j++) { //for each player + dealer
			Sleep(104); //wait a reasonable time
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

int buttonPress(int function) {
	//a function called to do whatever any UIbutton may want to do
	//it is in the main file to simplify function parameters
	//if it were in button.cpp for example, any button affecting a deck would need to be passed in with the function, and potentially passed back on return
	//but not all button functions will need the same parameters, so that would be a very messy function, and it leads to confusing or circular includes because of custom objects
	//Probably didn't need explaining but oh well
	vector<Card> handCopy = playerHand.getCards();
	switch (function) {
	case -1: //do nothing
		break;
	case 0: //enter the blackjack setup scene
		gamemode = 1;
		return 1;
		break;
	case 1: //enter the solitaire setup scene
		gamemode = 3;
		return 1;
		break;
	case 2: //exit the game
		gamemode = 5;
		return 1;
		break;
	case 3: //"hit" in blackjack
		drawnCard = standardDeck.drawTopCard();
		drawnCard.flip();
		playerHand.placeCardAtTop(drawnCard);
		break;
	case 4: //"stand" in blackjack
		whoseTurn++;
		break;
	case 5:
		if (handCopy.size() == 2 && handCopy[0].getValue() == handCopy[1].getValue()) {
			playerHand.drawTopCard();
			drawnCard = standardDeck.drawTopCard();
			drawnCard.flip();
			playerHand.placeCardAtTop(drawnCard);
		}
		else {
			cout << "\a";
		}
		break;
	case 6:
		gamemode = 0;
	default:
		break;
	}
	return 0;
}

int main() {
	//Buttons
	///mainMenu
	Button buttonBlackJack = Button(0, 10, "Blackjack", 0);
	Button buttonSolitaire = Button(0, 13, "Solitaire", 1);
	Button exitGame = Button(0, 16, "Exit", 2);
	///blackjack in-game
	Button hitButton = Button(65, 37, "Hit", 3);
	Button standButton = Button(85, 37, "Stand", 4);
	Button splitButton = Button(105, 37, "Split", 5);
	Button turnButton = Button(85, 37, "Next", -1);
	Button keepPlaying = Button(65, 37, "Yes", 0);
	Button returnToMenu = Button(85, 37, "No", 6);
	//UIs
	UserInterface mainMenu = UserInterface(vector<vector<Button>>{ { buttonBlackJack }, { buttonSolitaire }, { exitGame } });
	UserInterface blackJackMenu = UserInterface(vector<vector<Button>>{{hitButton, standButton, splitButton}});
	UserInterface blankMenu = UserInterface(vector<vector<Button>>{{turnButton}});
	UserInterface postBlackJackMenu = UserInterface(vector<vector<Button>>{{keepPlaying, returnToMenu}});
	int currentTotal;
	init();

	UserInterface currentUI;
	

	while (true){ //main game loop
		switch (gf::gamemode) {
		case 0: //shows the main menu
			currentUI.copyButtons(&mainMenu);
			drawBigSpade();
			break;
		case 1:
			//preparing the Deck on the table
			standardDeck = resetDeck; //resets the deck and everybody's hand
			playerHand = Deck(vector<Card>{});
			dealerHand = Deck(vector<Card>{});
			ai1Hand = Deck(vector<Card>{});
			ai2Hand = Deck(vector<Card>{});
			ai3Hand = Deck(vector<Card>{});
			ai4Hand = Deck(vector<Card>{});
			standardDeck.shuffle(); //shuffles the deck
			standardDeck.hideAll(); //puts the deck face down
			standardDeck.flipSpecific(0, true); //puts the last card face up
			standardDeck.stack(1, 1); //puts the deck in the top left corner

			standardDeck.renderAll(); 
			firstDeal(&standardDeck, &dealerHand, &playerHand, &ai1Hand, &ai2Hand, &ai3Hand, &ai4Hand); //deals 2 cards to each player
			turnOrder = { &ai1Hand, &ai2Hand, &playerHand, &ai3Hand, &ai4Hand, &dealerHand }; //defines the turn order
			gamemode = 2;
			currentUI.copyButtons(&blankMenu);
			whoseTurn = 1;
			break;
		case 2:
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
			switch (whoseTurn) {
			case 0: //first ai
				currentUI.copyButtons(&blankMenu);
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
				currentUI.copyButtons(&blankMenu);
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
			case 2:
				currentUI.copyButtons(&blackJackMenu);
				break;
			case 3: //third ai (after the player)
				currentUI.copyButtons(&blankMenu);
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
				currentUI.copyButtons(&blankMenu);
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
				currentUI.copyButtons(&blankMenu);
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
			case 6:
				currentUI.copyButtons(&postBlackJackMenu);
				bool playerCanWin = true;
				if (playerHand.blackJackValue(1) > 21) {
					playerCanWin = false;
				}
				for (int i = 0; i < turnOrder.size(); i++) {
					turnOrder[i]->showAll();
					if (turnOrder[i]->blackJackValue(1) > playerHand.blackJackValue(1) && turnOrder[i]->blackJackValue(1) <= 21) { //if any non-busted ai is higher value than the player
						playerCanWin = false;
					}
					else if (turnOrder[i]->blackJackValue(1) > playerHand.blackJackValue(1) && turnOrder[i]->blackJackValue(1) <= 21) {

					}
				}
				if (dealerHand.blackJackValue(1) == 21) {
					playerCanWin = false;
				}
				coords(75, 25);
				if (playerCanWin) {
					cout << "You win!";
				}
				else {
					cout << "You lose!";
				}
			}
			//rendering
			coords(0, 0);
			cout << "Deck";
			cout << currentUI.getCurrentlySelected().getText();
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
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
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
			break;
		case 5:
			return 1;
		}
		currentUI.renderUI();
		//input processing
		int ch = _getch(); //gets the latest keyboard input
		if (ch == 224) { //since arrow keys technically give 2 inputs, this prunes the first, useless one
			ch = _getch(); //then gets the real arrow value
			switch (ch) { //this switch is nested here so it will only respond to presses of the arrow keys
			case 72: //up arrow
				currentUI.selectionMoveUp();
				break;
			case 80: //down arrow
				currentUI.selectionMoveDown();
				break;
			case 75: //left arrow
				currentUI.selectionMoveLeft();
				break;
			case 77: //right arrow
				currentUI.selectionMoveRight();
				break;
			default:
				break;
			}
		}
		else { //this switch responds to everything else
			switch (ch) {
			case 13: //enter
				buttonPress(currentUI.getCurrentlySelected().getFunctionID()); //triggers the specfic function for whichever button is selected
				break;
			default:
				break;
			}
		}
		clearScreen();
	}
	return 1;
}