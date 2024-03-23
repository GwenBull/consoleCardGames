#include "Card.h"
#include "Deck.h"
#include "UserInterface.h"
using namespace std;
using namespace gf;

//Global constants
///blackJack Coordinates of various things
const vector<vector<int>> talkCoords = { {25, 10}, {50, 20}, {75, 29}, {100, 20}, {125, 10}, {75, 0} }; //the coordinates of each player's text box
const vector<vector<int>> handCoords = { {25, 11}, {50, 21}, {75, 30}, {100, 21}, {125, 11}, {75, 1} }; //the coordinates of each player's cards
const vector<vector<int>> scoreCoords = { {25, 15}, {50, 25}, {75, 34}, {100, 25}, {125, 15}, {75, 5} }; //the coordinates of each player's score display
///solitaire coordinates of various things
const vector<vector<int>> stackCoords = { {14, 1}, {21, 1}, {28, 1}, {35, 1}, {42, 1}, {49, 1}, {56, 1} }; //the coordinates of the active play card stacks
const vector<vector<int>> sortCoords = {}; //the coordinates of where each sorted card suit goes

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

bool aiTurn(int whichPlayer, int softLimit, Deck* standardDeck, vector<Deck*> turnOrder) { //returns true or false depending on if the AI took a card
	Card drawnCard;
	int currentTotal = turnOrder[whichPlayer]->blackJackValue(1); //get AI's current hand value
	if (currentTotal < softLimit && standardDeck->getCards().size() > 1) { //if the AI is below it's comfortable limit and there's cards left in the deck
		drawnCard = standardDeck->drawTopCard(); //have a card drawn
		drawnCard.flip();
		turnOrder[whichPlayer]->placeCardAtTop(drawnCard); //and added to their hand
		gf::coords(talkCoords[whichPlayer][0], talkCoords[whichPlayer][1]);
		cout << "Hit"; //announce that it's hit for a new card
		return true;
	}
	else if (currentTotal <= 21) { //else if it's above it's comfortable limit
		gf::coords(talkCoords[whichPlayer][0], talkCoords[whichPlayer][1]);
		cout << "Stand"; //announce that it's finished it's turn
	}
	else { //By logical elimination, this means they're bust
		gf::coords(talkCoords[whichPlayer][0], talkCoords[whichPlayer][1]);
		turnOrder[whichPlayer]->showAll(); //put hand face up
		cout << "Bust"; //announce bust
	}
	return false;
}

int readInput(UserInterface* currentUI) {
	int function = -1;
	int ch = _getch();
	if (ch == 224) { //since arrow keys technically give 2 inputs, this prunes the first, useless one
		ch = _getch(); //then gets the real arrow value
		switch (ch) { //this switch is nested here so it will only respond to presses of the arrow keys
		case 72: //up arrow
			currentUI->selectionMoveUp();
			break;
		case 80: //down arrow
			currentUI->selectionMoveDown();
			break;
		case 75: //left arrow
			currentUI->selectionMoveLeft();
			break;
		case 77: //right arrow
			currentUI->selectionMoveRight();
			break;
		default:
			break;
		}
	}
	else { //this switch responds to everything else
		switch (ch) {
		case 13: //enter
			function = currentUI->getCurrentlySelected().getFunctionID(); //triggers the specfic function for whichever button is selected
			break;
		default:
			break;
		}
	}
	return function;
}

int main() {
	int function = 0;
	//Global variables
	int whoseTurn = 0;
	bool drewQuery = false; //for blackjack, used as a flag for whether the current AI has drawn a card on this loop of main, so that the loop where it would display "Stand" or "Bust" doesn't get skipped

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

	//card vars with names general enough I can use them across gamemodes
	Card drawnCard;
	Card compCard;
	Deck standardDeck;
	Deck playerHand;
	//blackjack specific card vars
	Deck dealerHand;
	Deck ai1Hand;
	Deck ai2Hand;
	Deck ai3Hand;
	Deck ai4Hand;
	vector<Deck*> turnOrder = { &ai1Hand, &ai2Hand, &playerHand, &ai3Hand, &ai4Hand, &dealerHand };
	//solitaire specific card vars
	Deck drawnDeck;
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
	Deck tempHand;

	//Buttons
	///mainMenu
	Button buttonBlackJack = Button(0, 10, "Blackjack", "toBlackJack");
	Button buttonSolitaire = Button(0, 13, "Solitaire", "toSolitaire");
	Button exitGame = Button(0, 16, "Exit", "quit");
	///blackjack in-game
	Button hitButton = Button(65, 37, "Hit", "hit");
	Button standButton = Button(85, 37, "Stand", "stand");
	Button splitButton = Button(105, 37, "Split", "split");
	Button turnButton = Button(85, 37, "Next", "null");
	Button keepPlaying = Button(65, 37, "Yes", "toBlackJack");
	Button returnToMenu = Button(85, 37, "No", "toMenu");
	///solitaire in-game
	////solitaire take
	Button drawMore = Button(1, 20, "Draw", "solitaireDraw");
	Button takeFromDeck = Button(1, 23, "Take", "takeFromDeck");
	Button takeS0 = Button(14, 23, "Take", "takeFromStack");
	Button takeS1 = Button(21, 23, "Take", "takeFromStack");
	Button takeS2 = Button(28, 23, "Take", "takeFromStack");
	Button takeS3 = Button(35, 23, "Take", "takeFromStack");
	Button takeS4 = Button(42, 23, "Take", "takeFromStack");
	Button takeS5 = Button(49, 23, "Take", "takeFromStack");
	Button takeS6 = Button(56, 23, "Take", "takeFromStack");
	////take pt2
	Button takeOne = Button(28, 26, "Take one", "stackTakeOne");
	Button takeAll = Button(42, 26, "Take all", "stackTakeAll");
	////solitaire place
	Button placeS0 = Button(14, 23, " 1 ", "placeOnStack");
	Button placeS1 = Button(21, 23, " 2 ", "placeOnStack");
	Button placeS2 = Button(28, 23, " 3 ", "placeOnStack");
	Button placeS3 = Button(35, 23, " 4 ", "placeOnStack");
	Button placeS4 = Button(42, 23, " 5 ", "placeOnStack");
	Button placeS5 = Button(49, 23, " 6 ", "placeOnStack");
	Button placeS6 = Button(56, 23, " 7 ", "placeOnStack");
	Button placeSort = Button(63, 23, "Sort", "placeOnSorted");
	Button placeUndo = Button(70, 23, "Put back", "undo");
	//UIs
	UserInterface mainMenu = UserInterface(vector<vector<Button>>{ { buttonBlackJack }, { buttonSolitaire }, { exitGame } });
	UserInterface blackJackMenu = UserInterface(vector<vector<Button>>{{hitButton, standButton, splitButton}});
	UserInterface blankMenu = UserInterface(vector<vector<Button>>{{turnButton}});
	UserInterface postBlackJackMenu = UserInterface(vector<vector<Button>>{{keepPlaying, returnToMenu}});
	UserInterface solitaireTakeMenu = UserInterface(vector<vector<Button>>{{drawMore}, {takeFromDeck, takeS0, takeS1, takeS2, takeS3, takeS4, takeS5, takeS6}});
	UserInterface solitaireTakeOptions = UserInterface(vector<vector<Button>>{{takeOne, takeAll}});
	UserInterface solitairePlaceMenu = UserInterface(vector<vector<Button>>{{placeS0, placeS1, placeS2, placeS3, placeS4, placeS5, placeS6, placeSort, placeUndo}});
	UserInterface currentUI; //the UI that holds a copy of whichever of the above is currently in use
	init();

	vector<Deck*> dealOrder = { &stack0, &stack1, &stack2, &stack3, &stack4, &stack5, &stack6 };
	vector<Deck*> stacks = { &standardDeck, &stack0, &stack1, &stack2, &stack3, &stack4, &stack5, &stack6 };
	int undoPlace = 0;
	int takeableCards;

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
			gamemode = 2;
			whoseTurn = -1;
			currentUI.copyButtons(&blankMenu);
			continue; //resets the game loop so that everything renders properly on the first loop of blackjack
		case 2: //blackjack gameloop
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
			switch (whoseTurn) {
			case -1:
				whoseTurn++; //gives a single cycle of "wait time" so the player doesn't miss anything
				break;
			case 0: //first ai
				currentUI.copyButtons(&blankMenu);
				drewQuery = aiTurn(0, 16, &standardDeck, turnOrder);
				if (!drewQuery){
					whoseTurn++;
				}
				break;
			case 1: //second ai
				currentUI.copyButtons(&blankMenu);
				drewQuery = aiTurn(1, 20, &standardDeck, turnOrder);
				if (!drewQuery) {
					whoseTurn++;
				}
				break;
			case 2:
				currentUI.copyButtons(&blackJackMenu);
				break;
			case 3: //third ai (after the player)
				currentUI.copyButtons(&blankMenu);
				drewQuery = aiTurn(3, 17, &standardDeck, turnOrder);
				if (!drewQuery) {
					whoseTurn++;
				}
				break;
			case 4: //fourth ai
				currentUI.copyButtons(&blankMenu);
				drewQuery = aiTurn(4, 15, &standardDeck, turnOrder);
				if (!drewQuery) {
					whoseTurn++;
				}
				break;
			case 5: //fifth ai (dealer)
				currentUI.copyButtons(&blankMenu);
				drewQuery = aiTurn(5, 16, &standardDeck, turnOrder);
				if (!drewQuery) {
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
		case 3: //solitaire setup
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
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
			continue; //makes everything render properly for the first loop of solitaire
		case 4: //solitaire gameloop
			if (playerHand.getCards().size() == 0) { //decide ui
				currentUI.copyButtons(&solitaireTakeMenu);
			}
			else {
				currentUI.copyButtons(&solitairePlaceMenu);
			}

			playerHand.spreadVert(70, 1);
			for (int i = 0; i < 7; i++) { //organise cards
				if (dealOrder[i]->getCards().size() > 10) {
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
			//cards
			standardDeck.renderAll();
			drawnDeck.spreadVertLimited(1, 10, min(3, drawnDeck.getCards().size()));
			drawnDeck.renderAll();
			stack0.renderAll();
			stack1.renderAll();
			stack2.renderAll();
			stack3.renderAll();
			stack4.renderAll();
			stack5.renderAll();
			stack6.renderAll();
			Card("diamonds", " ", "dGrey", 63, 1).render();
			Card("clubs", " ", "lGrey", 63, 7).render();
			Card("hearts", " ", "dGrey", 63, 13).render();
			Card("spades", " ", "lGrey", 63, 19).render();
			diamonds.renderAll();
			clubs.renderAll();
			hearts.renderAll();
			spades.renderAll();
			playerHand.renderAll();

			break;
		case 5: //quit game
			return 1;
		}
		currentUI.renderUI();
		//input processing
		function = readInput(&currentUI);

		//when enter key is registered above, the function var is set to which
		buttonFunctions:
		switch (function) {
		case 0: //do nothing
			break;
		case 1: //enter the blackjack setup scene
			standardDeck = resetDeck;
			gamemode = 1;
			break;
		case 2: //enter the solitaire setup scene
			gamemode = 3;
			break;
		case 3: // return to menu
			gamemode = 0;
			break;
		case 4: //exit the game
			gamemode = 5;
			break;
		case 5: //"hit" in blackjack and picking up the top unstacked/unsorted card in solitaire 
			if (standardDeck.getCards().size() > 1) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.setFace(true);
				playerHand.placeCardAtTop(drawnCard);
				break;
			}
			break;
		case 6: //"stand" in blackjack
			whoseTurn++;
			break;
		case 7: //"split" in blackjack
			if (playerHand.getCards().size() == 2 && values[playerHand.getCards()[0].getValue()] == values[playerHand.getCards()[1].getValue()]) {
				playerHand.drawTopCard();
				drawnCard = standardDeck.drawTopCard();
				drawnCard.flip();
				playerHand.placeCardAtTop(drawnCard);
				break;
			}
			break;
		case 8: //turn over 3 cards in solitaire
			if (standardDeck.getCards().size() > 0) {
				takeableCards = min(3, standardDeck.getCards().size());
				for (int i = 0; i < takeableCards; i++) {
					drawnCard = standardDeck.drawTopCard();
					drawnCard.setFace(true);
					drawnDeck.placeCardAtTop(drawnCard);
				}
			}
			else {
				takeableCards = drawnDeck.getCards().size();
				for (int i = 0; i < takeableCards; i++) {
					drawnCard = drawnDeck.drawTopCard();
					drawnCard.setFace(false);
					standardDeck.placeCardAtTop(drawnCard);
				}
				standardDeck.stack(1, 1);
			}
			break;
		case 9: //take a card from the deck in solitaire
			drawnCard = drawnDeck.drawTopCard();
			playerHand.placeCardAtTop(drawnCard);
			undoPlace = 0;
			break;
		case 10: //take from a solitaire stack
			undoPlace = currentUI.getSelectionVal()[0];
			while (true) { //creates a temporary gameloop to allow selection between the "take one" and "take all" without affecting anything that was already on the screen
				currentUI.copyButtons(&solitaireTakeOptions);
				rect(28, 26, 52, 3, "whiteText"); //rectangle that covers only these new elements instead of clearing the whole screen
				currentUI.renderUI(); //draw the UI
				function = readInput(&currentUI);
				if (function != -1) { //once an option actually entered,
					goto buttonFunctions; //takes the function number back to the main game loop
				}
			}
			break;
		case 11: //only taking one card from a stack
			drawnCard = stacks[undoPlace]->drawTopCard();
			playerHand.placeCardAtTop(drawnCard);
			break;
		case 12: //taking all face up cards
			takeableCards = stacks[undoPlace]->getCards().size();
			for (int i = 0; i < takeableCards; i++) {
				if (stacks[undoPlace]->getCards()[stacks[undoPlace]->getCards().size() - 1].getFace() == true) {
					drawnCard = stacks[undoPlace]->drawTopCard();
					playerHand.placeCardAtTop(drawnCard);
				}
			}
			break;
		case 13: //"Put back" button in solitaire, returns player hand to where it was taken from without changing anything else
			if (undoPlace > 0) {
				takeableCards = playerHand.getCards().size();
				for (int i = 0; i < takeableCards; i++) {
					drawnCard = playerHand.drawTopCard();
					stacks[undoPlace]->placeCardAtTop(drawnCard);
				}
			}
			else {
				drawnCard = playerHand.drawTopCard();
				drawnDeck.placeCardAtTop(drawnCard);
			}
			break;
		case 14: //place all your held cards onto a stack
			takeableCards = playerHand.getCards().size();
			for (int i = 0; i < takeableCards; i++) {
				drawnCard = playerHand.drawTopCard();
				if (drawnCard.getValue() == "K") { //if the card to be placed is a king
					if (stacks[currentUI.getSelectionVal()[0] + 1]->getCards().size() == 0) {
						stacks[currentUI.getSelectionVal()[0] + 1]->placeCardAtTop(drawnCard);
					}
				}
				else if (stacks[currentUI.getSelectionVal()[0] + 1]->getCards().size() > 0){ //if there's a stack at the attempted position
					compCard = stacks[currentUI.getSelectionVal()[0] + 1]->getCards()[stacks[currentUI.getSelectionVal()[0] + 1]->getCards().size() - 1];
					if (compCard.getFace() == true){ //if the stack has a face-up top card
						if (solitaireValues[drawnCard.getValue()] == solitaireValues[compCard.getValue()] - 1) { //if the card to place is one less than the card to be placed on
							if (drawnCard.getColour() != compCard.getColour()) { //and their colours don't match
								stacks[currentUI.getSelectionVal()[0] + 1]->placeCardAtTop(drawnCard); //place the top card from the player hand
							}
							else {
								playerHand.placeCardAtTop(drawnCard);
							}
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
						}
					}
					else {
						playerHand.placeCardAtTop(drawnCard);
					}
				}
				else {
					playerHand.placeCardAtTop(drawnCard);
				}
			}
			if (undoPlace > 0 && stacks[undoPlace]->getCards().size() > 0 && playerHand.getCards().size() == 0) { //once your cards have been placed 
				stacks[undoPlace]->flipSpecific(stacks[undoPlace]->getCards().size() - 1, true); //reveal the next card from the stack that it was taken from
			}
			break;
		case 15: //sort your held card into its suit stack
			if (playerHand.getCards().size() == 1) { //only allow when the player only has one card
				drawnCard = playerHand.drawTopCard();
				if (drawnCard.getHouse() == "diamonds") { //if your card is a diamond
					if (diamonds.getCards().size() == 0) { //if the diamonds are empty
						if (drawnCard.getValue() == "A") { //and yours is an ace
							diamonds.placeCardAtTop(drawnCard); //start the diamonds stack
						}
						else { //if yours isn't an ace
							playerHand.placeCardAtTop(drawnCard); //put it back in your hand
							break; //skip the rest of this function
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[diamonds.getCards()[diamonds.getCards().size() - 1].getValue()]) { //if yours is 1 above the current top sorted
							diamonds.placeCardAtTop(drawnCard); //add yours on top
						}
						else { //if it isn't the next value
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					diamonds.stack(63, 1);
				}
				else if (drawnCard.getHouse() == "clubs") { //operates the same as the diamonds
					if (clubs.getCards().size() == 0) {
						if (drawnCard.getValue() == "A") {
							clubs.placeCardAtTop(drawnCard);
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[clubs.getCards()[clubs.getCards().size() - 1].getValue()]) {
							clubs.placeCardAtTop(drawnCard);
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					clubs.stack(63, 7);
				}
				else if (drawnCard.getHouse() == "hearts") {
					if (hearts.getCards().size() == 0) {
						if (drawnCard.getValue() == "A") {
							hearts.placeCardAtTop(drawnCard);
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[hearts.getCards()[hearts.getCards().size() - 1].getValue()]) {
							hearts.placeCardAtTop(drawnCard);
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					hearts.stack(63, 13);
				}
				else if (drawnCard.getHouse() == "spades") {
					if (spades.getCards().size() == 0) {
						if (drawnCard.getValue() == "A") {
							spades.placeCardAtTop(drawnCard);
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[spades.getCards()[spades.getCards().size() - 1].getValue()]) {
							spades.placeCardAtTop(drawnCard);
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					spades.stack(63, 19);
				}
				//if any successful placement was made
				if (undoPlace > 0) {
					stacks[undoPlace]->flipSpecific(stacks[undoPlace]->getCards().size() - 1, true); //show the next card on the stack where the sorted card was originally
				}
			}
			break;
		default: //unregistered button
			break;
		}

		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, 1283, 727, TRUE); //forces the window to stay at 720p (maybe a bad idea)
		//Sleep(1000);
		clearScreen();
	}
	return 1;
}
