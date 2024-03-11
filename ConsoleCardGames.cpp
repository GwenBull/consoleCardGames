#include "Card.h"
#include "Deck.h"
#include "UserInterface.h"
using namespace std;
using namespace gf;

//Global variables
//Window control
int whiteText = 7;
int whoseTurn = 0;

//blackJack  Coordinates of various things
vector<vector<int>> talkCoords = { {25, 10}, {50, 20}, {75, 29}, {100, 20}, {125, 10}, {75, 0} };
vector<vector<int>> handCoords = { {25, 11}, {50, 21}, {75, 30}, {100, 21}, {125, 11}, {75, 1} };
vector<vector<int>> scoreCoords = { {25, 15}, {50, 25}, {75, 34}, {100, 25}, {125, 15}, {75, 5} };
vector<Deck*> turnOrder;
//solitaire coordinates of various things
vector<vector<int>> stackCoords = { {14, 1}, {21, 1}, {28, 1}, {35, 1}, {42, 1}, {49, 1}, {56, 1} };
vector<vector<int>> sortCoords = {};

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
}); //I don't care. This works

//card vars with names general enough I canuse them across gamemodes
Card drawnCard;
Deck standardDeck;
Deck playerHand;
//blackjack specific card vars
Deck dealerHand;
Deck ai1Hand;
Deck ai2Hand;
Deck ai3Hand;
Deck ai4Hand;
//patience specific card vars
Deck stack0;
Deck stack1;
Deck stack2;
Deck stack3;
Deck stack4;
Deck stack5;
Deck stack6;
Deck diamonds;
Deck clubs;
Deck hearts;
Deck spades;

UserInterface currentUI;

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
	//for (int i = 0; i < 256; i++) {
	//	SetConsoleTextAttribute(hConsole, i);
	//	cout << i << ": " << char(i) << "\n";
	//}
	//_getch();
	clearScreen();
}

void firstDeal(Deck* mainPile, Deck* dealer, Deck* player, Deck* ai1, Deck* ai2, Deck* ai3, Deck* ai4) {
	Card drawnCard;
	vector<Deck*> dealOrder = { ai1, ai2, player, ai3, ai4, dealer };
	for (int i = 0; i < 2; i++) { //for the 2 initial cards per player
		for (int j = 0; j < 6; j++) { //for each player + dealer
			Sleep(208); //wait a reasonable time
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

void solitaireSetup(Deck* mainPile, Deck* s0, Deck* s1, Deck* s2, Deck* s3, Deck* s4, Deck* s5, Deck* s6) {
	Card drawnCard;
	vector<Deck*> dealOrder = { s0, s1, s2, s3, s4, s5, s6 };
	for (int i = 0; i < 7; i++) { //for the 7 initital stacks
		for (int j = i; j < 7; j++) { //for the (up to) 7 cards per stack
			Sleep(208); //wait a reasonable time
			drawnCard = mainPile->drawTopCard(); //draw a card
			dealOrder[j]->placeCardAtTop(drawnCard); //deal the card
			dealOrder[j]->spreadVert(stackCoords[j][0], stackCoords[j][1]); //place the stack at the right place
			if (i == j) { //if dealt to the top of a stack
				dealOrder[j]->flipSpecific(dealOrder[j]->getCards().size() - 1, true); //flip over
			}
			dealOrder[j]->renderAll(); //draw each stack
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
	vector<Deck*> stacks = { &standardDeck, &stack0, &stack1, &stack2, &stack3, &stack4, &stack5, &stack6 };
	Card drawnCard;
	switch (function) {
	case -1: //do nothing
		return 1;
	case 0: //enter the blackjack setup scene
		standardDeck = resetDeck;
		gamemode = 1;
		return 1;
	case 1: //enter the solitaire setup scene
		gamemode = 3;
		return 1;
	case 2: //exit the game
		gamemode = 5;
		return 1;
	case 3: //"hit" in blackjack and picking up the top unstacked/unsorted card in patience 
		if (standardDeck.getCards().size() > 1) {
			drawnCard = standardDeck.drawTopCard();
			drawnCard.setFace(true);
			playerHand.placeCardAtTop(drawnCard);
			return 1;
		}
		break;
	case 4: //"stand" in blackjack
		whoseTurn++;
		return 1;
	case 5: //"split" in blackjack
		if (handCopy.size() == 2 && values[handCopy[0].getValue()] == values[handCopy[1].getValue()]) {
			playerHand.drawTopCard();
			drawnCard = standardDeck.drawTopCard();
			drawnCard.flip();
			playerHand.placeCardAtTop(drawnCard);
			return 1;
		}
		break;
	case 6: // return to menu
		gamemode = 0;
		return 1;
	case 7: //restarts blackjack without resetting the deck
		gamemode = 1;	
		if (standardDeck.getCards().size() < 13) { //unless there's not enough cards to deal the inital hands
			standardDeck = resetDeck;
			return 0;
		}
		return 1;
	case 8: //takes a single card in patience (specifically for patience because there's different logic needed than the blackjack draw
		if (standardDeck.getCards().size() >= 1) {
			drawnCard = standardDeck.drawTopCard();
			drawnCard.setFace(true);
			playerHand.placeCardAtTop(drawnCard);
			return 1;
		}
		break;
	case 9: //takes every face-up card from a specific patience stack
		break;
	case 10: //places your hand at the targeted stack
		playerHand.reverseCards(); //reverses the order of the held cards for ease of iteration
		for (int i = 0; i < playerHand.getCards().size(); i++) {
			drawnCard = playerHand.drawTopCard(); //take the next to be placed
			stacks[currentUI.getSelectionVal()[0]]->placeCardAtTop(drawnCard); //and place it in the stack above the pressed button
			return 1;
		}
		break;
	default: //unregistered button
		break;
	}
	cout << "\a"; //if a function has got this far, it has not been able to complete it's intended function
	return 0; //return a 0; I'll likely never read the return values from this function but it is useful just in case
}

void aiTurn(int whichPlayer, int softLimit) {
	int currentTotal = turnOrder[whichPlayer]->blackJackValue(1);
	if (currentTotal < softLimit && standardDeck.getCards().size() > 1) {
		drawnCard = standardDeck.drawTopCard();
		drawnCard.flip();
		turnOrder[whichPlayer]->placeCardAtTop(drawnCard);
		gf::coords(talkCoords[whichPlayer][0], talkCoords[whichPlayer][1]);
		cout << "Hit";
	}
	else if (currentTotal <= 21) {
		gf::coords(talkCoords[whichPlayer][0], talkCoords[whichPlayer][1]);
		cout << "Stand";
		whoseTurn++;
	}
	else {
		gf::coords(talkCoords[whichPlayer][0], talkCoords[whichPlayer][1]);
		turnOrder[whichPlayer]->showAll();
		cout << "Bust";
		whoseTurn++;
	}

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
	Button keepPlaying = Button(65, 37, "Yes", 7);
	Button returnToMenu = Button(85, 37, "No", 6);
	///solitaire in-game
	////solitaire take
	Button drawMore = Button(1, 5, "Draw", 8);
	Button takeFromDeck = Button(1, 28, "Take", 9);
	Button takeS0 = Button(14, 28, "Take", 12);
	Button takeS1 = Button(21, 28, "Take", 12);
	Button takeS2 = Button(28, 28, "Take", 12);
	Button takeS3 = Button(35, 28, "Take", 12);
	Button takeS4 = Button(42, 28, "Take", 12);
	Button takeS5 = Button(49, 28, "Take", 12);
	Button takeS6 = Button(56, 28, "Take", 12);
	////solitaire place
	Button placeDeck = Button(1, 28, "Deck", 10);
	Button placeS0 = Button(14, 28, "  1  ", 10);
	Button placeS1 = Button(21, 28, "  2  ", 10);
	Button placeS2 = Button(28, 28, "  3  ", 10);
	Button placeS3 = Button(35, 28, "  4  ", 10);
	Button placeS4 = Button(42, 28, "  5  ", 10);
	Button placeS5 = Button(49, 28, "  6  ", 10);
	Button placeS6 = Button(56, 28, "  7  ", 10);
	Button placeSort = Button(63, 28, "Sort", 11);
	//UIs
	UserInterface mainMenu = UserInterface(vector<vector<Button>>{ { buttonBlackJack }, { buttonSolitaire }, { exitGame } });
	UserInterface blackJackMenu = UserInterface(vector<vector<Button>>{{hitButton, standButton, splitButton}});
	UserInterface blankMenu = UserInterface(vector<vector<Button>>{{turnButton}});
	UserInterface postBlackJackMenu = UserInterface(vector<vector<Button>>{{keepPlaying, returnToMenu}});
	UserInterface solitaireTakeMenu = UserInterface(vector<vector<Button>>{{drawMore}, {takeFromDeck, takeS0, takeS1, takeS2, takeS3, takeS4, takeS5, takeS6}});
	UserInterface solitairePlaceMenu = UserInterface(vector<vector<Button>>{{placeDeck, placeS0, placeS1, placeS2, placeS3, placeS4, placeS5, placeS6}});
	int currentTotal;
	init();

	vector<Deck*> dealOrder = { &stack0, &stack1, &stack2, &stack3, &stack4, &stack5, &stack6};

	while (true){ //main game loop
		switch (gf::gamemode) {
		case 0: //shows the main menu
			currentUI.copyButtons(&mainMenu);
			drawBigSpade();
			coords(0, 42);
			cout << "Use the arrow keys to move, use enter to select";
			break;
		case 1: //blackjack setup
			//preparing the Deck on the table
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
			whoseTurn = 0;
			currentUI.copyButtons(&blankMenu);
			break;
		case 2: //blackjack gameloop
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
			switch (whoseTurn) {
			case 0: //first ai
				currentUI.copyButtons(&blankMenu);
				aiTurn(0, 16);
				break;
			case 1: //second ai
				currentUI.copyButtons(&blankMenu);
				aiTurn(1, 20);
				break;
			case 2:
				currentUI.copyButtons(&blackJackMenu);
				break;
			case 3: //third ai (after the player)
				currentUI.copyButtons(&blankMenu);
				aiTurn(3, 17);
				break;
			case 4: //fourth ai
				currentUI.copyButtons(&blankMenu);
				aiTurn(4, 15);
				break;
			case 5: //fifth ai (dealer)
				currentUI.copyButtons(&blankMenu);
				aiTurn(5, 16);
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
				if (dealerHand.blackJackValue(1) <= 21 && dealerHand.blackJackValue(1) >= playerHand.blackJackValue(1)) {
					playerCanWin = false;
				}
				coords(75, 25);
				if (playerCanWin) {
					cout << "You win!";
				}
				else {
					cout << "You lose!";
				}
				coords(75, 26);
				cout << "Play again?";
			}
			//rendering
			coords(1, 0);
			cout << "Deck";
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
		case 3: //patience setup
			//empty the suit-sorting-stacks
			diamonds = Deck(vector<Card>{});
			clubs = Deck(vector<Card>{});
			hearts = Deck(vector<Card>{});
			spades = Deck(vector<Card>{});
			//empty the game area stacks
			stack0 = Deck(vector<Card>{});
			stack1 = Deck(vector<Card>{});
			stack2 = Deck(vector<Card>{});
			stack3 = Deck(vector<Card>{});
			stack4 = Deck(vector<Card>{});
			stack5 = Deck(vector<Card>{});
			stack6 = Deck(vector<Card>{});
			//empty the player held cards (should be empty already but safety first
			playerHand = Deck(vector<Card>{});

			standardDeck = resetDeck;
			standardDeck.shuffle(); //shuffles the deck
			standardDeck.hideAll(); //puts the deck face down
			standardDeck.flipSpecific(0, true); //puts the last card face up
			standardDeck.stack(1, 1); //puts the deck in the top left corner
			solitaireSetup(&standardDeck, &stack0, &stack1, &stack2, &stack3, &stack4, &stack5, &stack6);
			gamemode = 4;
			currentUI.copyButtons(&solitaireTakeMenu);
			standardDeck.renderAll();
			break;
		case 4: //patience gameloop
			if (playerHand.getCards().size() == 0) { //decide ui
				currentUI.copyButtons(&solitaireTakeMenu);
			}
			else {
				currentUI.copyButtons(&solitairePlaceMenu);
			}

			playerHand.spreadVert(70, 1);
			for (int i = 0; i < 7; i++) { //organise cards
				if (dealOrder[i]->getCards().size() > 12) {
					dealOrder[i]->spreadVertCompressed(stackCoords[i][0], stackCoords[i][1]);
				}
				else {
					dealOrder[i]->spreadVert(stackCoords[i][0], stackCoords[i][1]);
				}
			}
			//Rendering
			coords(1, 0);
			cout << "Deck";
			coords(63, 0);
			cout << "Sort";
			coords(70, 0);
			cout << "Held";
			//fake cards
			Card("diamonds", " ", "dGrey", 63, 1).render();
			Card("clubs", " ", "lGrey", 63, 7).render();
			Card("hearts", " ", "dGrey", 63, 13).render();
			Card("spades", " ", "lGrey", 63, 19).render();
			//cards
			standardDeck.renderAll();
			stack0.renderAll();
			stack1.renderAll();
			stack2.renderAll();
			stack3.renderAll();
			stack4.renderAll();
			stack5.renderAll();
			stack6.renderAll();
			playerHand.renderAll();

			break;
		case 5: //quit game
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
		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, 1283, 727, TRUE); //makes the window 720p (the drawable dimensions are 154 by 40 ish)
		clearScreen();
	}
	return 1;
}