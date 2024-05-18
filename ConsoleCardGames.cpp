#include "Card.h"
#include "Deck.h"
#include "UserInterface.h"
using namespace std;
using namespace gf;

//Global constants
///blackJack Coordinates of various things
const vector<vector<int>> talkCoords = { {25, 10}, {50, 20}, {75, 28}, {100, 20}, {125, 10}, {75, 0} }; //the coordinates of each player's text box
const vector<vector<int>> handCoords = { {25, 11}, {50, 21}, {75, 29}, {100, 21}, {125, 11}, {75, 1} }; //the coordinates of each player's cards
const vector<vector<int>> scoreCoords = { {25, 15}, {50, 25}, {75, 33}, {100, 25}, {125, 15}, {75, 5} }; //the coordinates of each player's score display
///solitaire coordinates of various things
const vector<vector<int>> stackCoords = { {14, 1}, {21, 1}, {28, 1}, {35, 1}, {42, 1}, {49, 1}, {56, 1} }; //the coordinates of the active play card stacks

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

void firstDealSolo(Deck* mainPile, Deck* dealer, Deck* player) {
	Card drawnCard;
	vector<Deck*> dealOrder = {player, player, player, dealer, dealer, dealer };
	for (int i = 0; i < 2; i++) { //for the 2 initial cards per player
		for (int j = 2; j < 6; j+=3) { //for each player + dealer
			Sleep(208); //wait a reasonable time
			drawnCard = mainPile->drawTopCard(); //draw a Card
			if (j == 2 || (j == 6 && i == 1)) { //if it's the player's Card or the dealers last Card
				drawnCard.flip(); //make it visible
			}
			if (j > 6) {
				break;
			}
			dealOrder[j]->placeCardAtTop(drawnCard); //deal the Card
			dealOrder[j]->spreadHoriz(handCoords[j][0], handCoords[j][1]); //put it in the right position
			dealOrder[j]->renderAll(); //render it
		}
	}
}

void showWins(int wins, int losses, int busts) {
	SetConsoleTextAttribute(hConsole, colours["whiteText"]);
	coords(75, 34);
	cout << "wins: " << wins;
	coords(75, 35);
	cout << "losses: " << losses << " (of which from busts: " << busts << ")";
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

void clearBlackJackButtons() {
	rect(65, 37, 47, 3, "whiteText");
}
void clearSolitaireText() {
	rect(1, 0, 73, 1, "whiteText");
}
void clearSolitaireDeck() {
	rect(1, 1, 5, 4, "whiteText");
}
void clearSolitaireDrawn() {
	rect(1, 10, 5, 8, "whiteText");
}
void clearSolitaireButtons() {
	rect(1, 23, 79, 6, "whiteText");
	rect(1, 20, 6, 3, "whiteText");
	rect(63, 29, 6, 5, "whiteText");
}
void clearSolitairePlay() {
	rect(14, 1, 47, 22, "whiteText");
}
void clearSolitaireSort() {
	rect(63, 1, 5, 22, "whiteText");
}
void clearSolitaireHand() {
	rect(70, 1, 5, 28, "whiteText");
}

int readInput(UserInterface* currentUI) {
	int function = -1;
	int x = currentUI->getCurrentlySelected().getX();
	int y = currentUI->getCurrentlySelected().getY(); 
	int w = currentUI->getCurrentlySelected().getText().length() + 2;
	int ch = _getch();
	if (ch == 224) { //since arrow keys technically give 2 inputs, this prunes the first one which is just a generic code to identify it as *any* of the arrow keys
		ch = _getch(); //then gets the real arrow value
		rect(x, y, w, 3, "whiteText");
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
			rect(x, y, w, 3, "whiteText");
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
	//blackjack specific vars
	int decks = 1;
	string groupType = "solo";
	int wins = 0;
	int losses = 0;
	int busts = 0;
	bool playerCanWin = true;
	Deck dealerHand;
	Deck ai1Hand;
	Deck ai2Hand;
	Deck ai3Hand;
	Deck ai4Hand;
	vector<Deck*> turnOrder = { &ai1Hand, &ai2Hand, &playerHand, &ai3Hand, &ai4Hand, &dealerHand };
	//solitaire specific vars
	int difficulty = 1;
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

	//solitaire bounce variables
	vector<int> directions; //the direction each card is going
	int nextCard; //the next card index to start animating

	//Buttons
	///mainMenu
	Button buttonBlackJack = Button(0, 10, "Blackjack", "toBlackJack");
	Button buttonSolitaire = Button(0, 13, "Solitaire", "toSolitaire");
	Button exitGame = Button(0, 16, "Exit", "quit");
	///blackjack settings
	Button oneDeck = Button(12, 10, "One Deck", "1Deck");
	Button twoDeck = Button(24, 10, "Two Deck", "2Deck");
	Button fourDeck = Button(36, 10, "Four Deck", "4Deck");
	Button solo = Button(12, 13, "1 V 1", "solo");
	Button group = Button(24, 13, "Group", "group");
	Button blackJackPlay = Button(12, 16, "Begin", "beginBlackJack");
	///blackjack in-game
	Button hitButton = Button(65, 37, "Hit", "hit");
	Button standButton = Button(85, 37, "Stand", "stand");
	Button splitButton = Button(105, 37, "Split", "split");
	Button turnButton = Button(85, 37, "Next", "null");
	Button keepPlaying = Button(65, 37, "Yes", "beginBlackJack");
	Button returnToMenu = Button(85, 37, "No", "toMenu");
	///solitaire settings
	Button easyDiff = Button(12, 13, "Easy", "easy");
	Button hardDiff = Button(24, 13, "Hard", "hard");
	Button solitairePlay = Button(12, 16, "Begin", "beginSolitaire");
	///solitaire in-game
	Button restart = Button(1, 26, "Restart", "toSolitaire");
	Button toMenu = Button(14, 26, "Exit", "toMenu");
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
	Button takeDmnd = Button(63, 23, "Take", "takeFromSort");
	Button takeClub = Button(63, 26, "Take", "takeFromSort");
	Button takeHart = Button(63, 29, "Take", "takeFromSort");
	Button takeSpad = Button(63, 32, "Take", "takeFromSort");
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
	UserInterface choicesBlackJack = UserInterface(vector<vector<Button>>{{oneDeck, twoDeck, fourDeck}, { solo, group }, { blackJackPlay }});
	UserInterface blackJackMenu = UserInterface(vector<vector<Button>>{{hitButton, standButton, splitButton}});
	UserInterface blankMenu = UserInterface(vector<vector<Button>>{{turnButton}});
	UserInterface postBlackJackMenu = UserInterface(vector<vector<Button>>{{keepPlaying, returnToMenu}});
	UserInterface choicesSolitaire = UserInterface(vector<vector<Button>>{{easyDiff, hardDiff}, { solitairePlay }});
	UserInterface solitaireTakeMenu = UserInterface(vector<vector<Button>>{{drawMore}, { takeFromDeck, takeS0, takeS1, takeS2, takeS3, takeS4, takeS5, takeS6, takeDmnd }, { restart, toMenu, takeClub }, { takeHart }, { takeSpad }});
	UserInterface solitaireTakeOptions = UserInterface(vector<vector<Button>>{{takeOne, takeAll}});
	UserInterface solitairePlaceMenu = UserInterface(vector<vector<Button>>{{placeS0, placeS1, placeS2, placeS3, placeS4, placeS5, placeS6, placeSort, placeUndo}});
	UserInterface postSolitaireMenu = UserInterface(vector<vector<Button>>{{restart, toMenu}});
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
		case 1: //blackjack group setup
			//preparing the Deck on the table
			clearScreen();
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
			standardDeck.renderAll();
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
				playerCanWin = true; //by default, assume the player might be the winner, then check every condition that could mean they've lost
				for (int i = 0; i < turnOrder.size(); i++) { //check against other players
					turnOrder[i]->showAll(); //reveal their cards
					if (turnOrder[i]->blackJackValue(1) > playerHand.blackJackValue(1) && turnOrder[i]->blackJackValue(1) <= 21 && playerCanWin) { //if any non-busted ai is higher value than the player, it's a loss
						playerCanWin = false;
						losses++;
					}
				}
				if (playerHand.blackJackValue(1) > 21 && playerCanWin) { //if they're busted, it's a loss
					playerCanWin = false;
					losses++;
					busts++;
				}
				else if (dealerHand.blackJackValue(1) <= 21 && dealerHand.blackJackValue(1) >= playerHand.blackJackValue(1) && playerCanWin) { //if the dealer is greater or equal than you, the house wins
					playerCanWin = false;
					losses++;
				}
				else if (playerCanWin) {
					wins++;
				}
				whoseTurn = 7;
				continue;
			case 7:
				currentUI.copyButtons(&postBlackJackMenu);
				coords(75, 25);
				if (playerCanWin) {
					cout << "You win!";
				}
				else {
					cout << "You lose!";
				}
				coords(75, 26);
				if (standardDeck.getCards().size() >= 12) {
					cout << "Keep going?";
				}
				else {
					
					cout << "Play again?";
				}
				break;
			}
			//rendering
			coords(1, 0);
			cout << "Deck";
			//cards
			if (standardDeck.getCards().size() == 1) {
				standardDeck.renderAll();
			}
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
			showWins(wins, losses, busts);
			//erase previous scores
			rect(scoreCoords[0][0], scoreCoords[0][1], 14, 1, "whiteText");
			rect(scoreCoords[1][0], scoreCoords[1][1], 14, 1, "whiteText");
			rect(scoreCoords[2][0], scoreCoords[2][1], 14, 1, "whiteText");
			rect(scoreCoords[3][0], scoreCoords[3][1], 14, 1, "whiteText");
			rect(scoreCoords[4][0], scoreCoords[4][1], 14, 1, "whiteText");
			rect(scoreCoords[5][0], scoreCoords[5][1], 14, 1, "whiteText");
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
			clearScreen();
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

			drawnDeck = Deck(vector<Card>{});
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
				if (playerHand.getCards().size() > 10) {
					playerHand.spreadVertCompressed(70, 1);
				}
				else {
					playerHand.spreadVert(70, 1);
				}
				currentUI.copyButtons(&solitairePlaceMenu);
			}
			for (int i = 0; i < 7; i++) { //organise cards
				if (dealOrder[i]->getCards().size() > 10) {
					dealOrder[i]->spreadVertCompressed(stackCoords[i][0], stackCoords[i][1]);
				}
				else {
					dealOrder[i]->spreadVert(stackCoords[i][0], stackCoords[i][1]);
				}
				dealOrder[i]->renderAll();
			}

			////test code - put all cards back in the main deck and sets everything up for the victory animation
			//standardDeck.copyAll(&stack0);
			//standardDeck.copyAll(&stack1);
			//standardDeck.copyAll(&stack2);
			//standardDeck.copyAll(&stack3);
			//standardDeck.copyAll(&stack4);
			//standardDeck.copyAll(&stack5);
			//standardDeck.copyAll(&stack6);
			//standardDeck.showAll();
			//clearScreen();
			//currentUI.copyButtons(&blankMenu);
			//nextCard = 0;
			//standardDeck.reverseCards();
			//for (int i = 0; i < 52; i++) {
			//	directions.push_back(1 + (rand() % 4));
			//	nextCard = 0;
			//}
			//gamemode = 6;
			//continue;
			////end of test code

			if (diamonds.getCards().size() == 13 && clubs.getCards().size() == 13 && hearts.getCards().size() == 13 && spades.getCards().size() == 13) { //if all of the cards are in their sorted stacks
				for (int i = 0; i < 13; i++) {
					drawnCard = diamonds.drawTopCard();
					standardDeck.placeCardAtTop(drawnCard);
					drawnCard = clubs.drawTopCard();
					standardDeck.placeCardAtTop(drawnCard);
					drawnCard = hearts.drawTopCard();
					standardDeck.placeCardAtTop(drawnCard);
					drawnCard = spades.drawTopCard();
					standardDeck.placeCardAtTop(drawnCard);
					//cards are now back in one deck, ordered descending from kings
				}
				for (int i = 0; i < 52; i++) {
					directions.push_back(1 + (rand() % 4)); //gives each card a random direction 1-4 meaning one of the diagonals
					nextCard = 0;
				}
				clearSolitaireButtons();
				gamemode = 6; //advance to the end screen
				continue;
			}

			//Rendering
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
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
			drawnDeck.spreadVertLimited(1, 10, min(3, drawnDeck.getCards().size()));
			drawnDeck.renderAll();
			diamonds.renderAll();
			clubs.renderAll();
			hearts.renderAll();
			spades.renderAll();
			playerHand.renderAll();

			break;
		case 5: //quit game
			return 1;
		case 6: //the solitaire bounce
			for (int i = 0; i < min(nextCard/16, 52); i++) { //iterate through (up to) the whole deck of cards
				int thisX = standardDeck.getCards()[i].getX();
				int thisY = standardDeck.getCards()[i].getY();
				//rect(thisX, thisY, 5, 4, "whiteText"); //removes the previous draw of the card, results in horrible flickering when all cards are moving, basically the same visual impact as clearScreen()
				if (directions[i] == 1) { //and move them according to their assigned direction
					standardDeck.moveCard(i, thisX + 1, thisY - 1);
					thisX++;
					thisY--;
					if (thisY == 0) { //check if it's hitting an edge/corner and change the direction
						directions[i] = 2;
					}
					if (thisX == 151) {
						directions[i] = 4;
					}
					if (thisY == 0 && thisX == 151) {
						directions[i] = 3;
					}
				}
				else if (directions[i] == 2) { //and move them according to their assigned direction
					standardDeck.moveCard(i, thisX + 1, thisY + 1);
					thisX++;
					thisY++;
					if (thisY == 39) { //check if it's hitting an edge/corner and change the direction
						directions[i] = 1;
					}
					if (thisX == 151) {
						directions[i] = 3;
					}
					if (thisY == 39 && thisX == 151) {
						directions[i] = 4;
					}
				}
				else if (directions[i] == 3) { //and move them according to their assigned direction
					standardDeck.moveCard(i, thisX - 1, thisY + 1);
					thisX--;
					thisY++;
					if (thisY == 39) { //check if it's hitting an edge/corner and change the direction
						directions[i] = 4;
					}
					if (thisX == 0) {
						directions[i] = 2;
					}
					if (thisY == 39 && thisX == 0) {
						directions[i] = 1;
					}
				}
				else if (directions[i] == 4) { //and move them according to their assigned direction
					standardDeck.moveCard(i, thisX - 1, thisY - 1);
					thisX--;
					thisY--;
					if (thisY == 0) { //check if it's hitting an edge/corner and change the direction
						directions[i] = 3;
					}
					if (thisX == 0) {
						directions[i] = 1;
					}
					if (thisY == 0 && thisX == 0) {
						directions[i] = 2;
					}
				}
				standardDeck.getCards()[i].render();
			}
			//standardDeck.renderAll();
			nextCard++; //nextcard tracks how many of the cards are allowed to move, and is also a counter for how many frames the animation has been moved
			if (nextCard < 416) {
				Sleep(13);
				continue; //bypasses the check for user input by going directly to the next iteration of the "while (true)", allowing the animation to play automatically
			}
			else { //once the animation has been running long enough, allow the player to options to restart/quit
				currentUI.copyButtons(&postSolitaireMenu);
				currentUI.renderUI(); //an override to make sure the UI is always drawn because "continue;" skips the unconditional call to "currentUI.renderUI()"
				if (_kbhit()) { //checks if you've pressed a button and breaks out to the unconditional part of the gameloop where input is processed
					break;
				}
				else { //if there's no keyboard hit detected allow the animation to continue
					Sleep(13);
					continue;
				}
			}
		case 7: //solo blackjack setup
			clearScreen();
			playerHand = Deck(vector<Card>{});
			dealerHand = Deck(vector<Card>{});
			standardDeck.shuffle(); //shuffles the deck
			standardDeck.hideAll(); //puts the deck face down
			standardDeck.flipSpecific(0, true); //puts the last card face up
			standardDeck.stack(1, 1); //puts the deck in the top left corner

			standardDeck.renderAll();
			firstDealSolo(&standardDeck, &dealerHand, &playerHand); //deals 2 cards to each player
			gamemode = 8;
			whoseTurn = -1;
			currentUI.copyButtons(&blankMenu);
			continue; //resets the game loop so that everything renders properly on the first loop of blackjack
		case 8: //solo blackjack
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
			switch (whoseTurn) {
			case -1:
				whoseTurn++; //gives a single cycle of "wait time" so the player doesn't miss anything
				break;
			case 0:
				currentUI.copyButtons(&blackJackMenu);
				break;
			case 1: //dealer
				currentUI.copyButtons(&blankMenu);
				if (playerHand.blackJackValue(0) <= 21) { //Dealer won't bother going if you've busted
					drewQuery = aiTurn(5, 16, &standardDeck, turnOrder);
				}
				else {
					drewQuery = false;
				}
				if (!drewQuery) {
					whoseTurn++;
				}
				break;
			case 2:
				currentUI.copyButtons(&postBlackJackMenu);
				playerCanWin = true; //by default, assume the player might be the winner, then check every condition that could mean they've lost
				dealerHand.showAll();
				if (playerHand.blackJackValue(1) > 21 && playerCanWin) { //if you're busted, it's a loss
					playerCanWin = false;
					losses++;
					busts++;
				}
				else if (dealerHand.blackJackValue(1) <= 21 && dealerHand.blackJackValue(1) >= playerHand.blackJackValue(1) && playerCanWin) { //if the dealer is greater or equal than you, the house wins
					playerCanWin = false;
					losses++;
				}
				else if (playerCanWin) {
					wins++;
				}
				whoseTurn = 3;
				continue;
			case 3:
				currentUI.copyButtons(&postBlackJackMenu);
				coords(75, 25);
				if (playerCanWin) { //if the player won
					cout << "You win!";
				}
				else {
					cout << "You lose!";
				}
				coords(75, 26);
				if (standardDeck.getCards().size() >= 4) { //change the prompt based on if there are enough cards in the deck for another deal
					cout << "Keep going?";
				}
				else {
					cout << "Play again?";
				}
				break;
			}
			//rendering
			coords(1, 0);
			cout << "Deck";
			//cards
			if (standardDeck.getCards().size() == 1) {
				standardDeck.renderAll();
			}
			playerHand.spreadHoriz(handCoords[2][0], handCoords[2][1]); //draws the players cards spread from position 10, 10
			playerHand.renderAll();
			dealerHand.spreadHoriz(handCoords[5][0], handCoords[5][1]);
			dealerHand.renderAll();
			showWins(wins, losses, busts);
			//erase previous scores
			rect(scoreCoords[2][0], scoreCoords[2][1], 14, 1, "whiteText");
			rect(scoreCoords[5][0], scoreCoords[5][1], 14, 1, "whiteText");
			//show hand values
			SetConsoleTextAttribute(hConsole, colours["whiteText"]);
			coords(scoreCoords[2][0], scoreCoords[2][1]);
			cout << "Total: " << playerHand.blackJackValue("string");
			coords(scoreCoords[5][0], scoreCoords[5][1]);
			cout << "Total: " << dealerHand.blackJackValue("string");
			break;
		case 9: //BlackJack options menu
			currentUI.copyButtons(&choicesBlackJack);
			coords(12, 9);
			if (currentUI.getSelectionVal()[1] == 0) { //work out the highlighted option and explain it
				if (currentUI.getSelectionVal()[0] == 0) {
					cout << "play with one deck (the deck will not be reset until there aren't enough cards left for a deal)  ";
				}
				else if (currentUI.getSelectionVal()[0] == 1) {
					cout << "play with two decks (the deck will not be reset until there aren't enough cards left for a deal) ";
				}
				else if (currentUI.getSelectionVal()[0] == 2) {
					cout << "play with four decks (the deck will not be reset until there aren't enough cards left for a deal)";
				}
			}
			else if (currentUI.getSelectionVal()[1] == 1) {
				if (currentUI.getSelectionVal()[0] == 0) {
					cout << "play alone against the dealer                                                                    ";
				}
				else if (currentUI.getSelectionVal()[0] == 1) {
					cout << "play with a group (of CPU opponents)                                                             "; //big spaces at the end to efficiently blank out the previously selected option
				}
			}
			else {
				cout << "Begin game                              ";
			}
			coords(48, 11);
			cout << "selected: " << decks;
			coords(48, 14);
			cout << "selected: " << groupType << " ";
			break;
		case 10: //Solitaire options menu
			currentUI.copyButtons(&choicesSolitaire);
			coords(12, 9);
			if (currentUI.getSelectionVal()[1] == 0) {
				if (currentUI.getSelectionVal()[0] == 0) {
					cout << "draw from the deck one card at a time   ";
				}
				else {
					cout << "draw from the deck three cards at a time";
				}
			}
			else {
				cout << "begin game                              ";
			}
			coords(36, 14);
			cout << "selected: " << vector<string>{"nullOpt", "easy", "nullOpt", "hard"}[difficulty];
			break;
		default:
			gamemode = 0;
			break;
		}
		currentUI.renderUI();
		//input processing
		function = readInput(&currentUI);

		//when enter key is registered in readInput(), the function var is set to the functionCode of whichever button was selected
		buttonFunctions:
		switch (function) {
		case 0: //do nothing
			break;
		case 1: //enter the blackjack options scene
			clearScreen();
			gamemode = 9;
			break;
		case 2: //enter the solitaire options scene
			clearScreen();
			gamemode = 10;
			break;
		case 3: // return to menu
			clearScreen();
			wins = 0;
			losses = 0;
			busts = 0;
			gamemode = 0;
			break;
		case 4: //exit the game
			gamemode = 5;
			break;
		case 5: //"hit" in blackjack and picking up the top unstacked/unsorted card in solitaire 
			if (standardDeck.getCards().size() > 1 && playerHand.getCards().size() < 20) {
				drawnCard = standardDeck.drawTopCard();
				drawnCard.setFace(true);
				playerHand.placeCardAtTop(drawnCard);
				break;
			}
			break;
		case 6: //"stand" in blackjack
			clearBlackJackButtons();
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
		case 8: //turn over 3 cards in solitaire (1 if easy mode)
			if (standardDeck.getCards().size() > 0) { //if there are cards to turn over
				takeableCards = min(difficulty, standardDeck.getCards().size()); //work out if you need to take less than three
				for (int i = 0; i < takeableCards; i++) {
					drawnCard = standardDeck.drawTopCard(); //take the cards
					drawnCard.setFace(true);
					drawnDeck.placeCardAtTop(drawnCard); //move them to the drawnDeck
				}
				clearSolitaireDeck();
			}
			else { //if the hidden deck is empty
				takeableCards = drawnDeck.getCards().size(); 
				for (int i = 0; i < takeableCards; i++) { //turn every drawn card back over
					drawnCard = drawnDeck.drawTopCard();
					drawnCard.setFace(false);
					standardDeck.placeCardAtTop(drawnCard); //and put them back in the deck
				}
				standardDeck.stack(1, 1); //put the deck on its spot
				clearSolitaireDrawn();
			}

			break;
		case 9: //take a card from the deck in solitaire
			if (drawnDeck.getCards().size() > 0) { //if there's a card available to move to your hand
				drawnCard = drawnDeck.drawTopCard();
				playerHand.placeCardAtTop(drawnCard); //take the top drawn card
				undoPlace = 0; //mark the deck as the place to put back to on "undo"
				clearSolitaireButtons();
				clearSolitaireDrawn();
			}
			break;
		case 10: //take from a solitaire stack
			undoPlace = currentUI.getSelectionVal()[0];
			while (true) { //creates a temporary gameloop to allow selection between the "take one" and "take all" without affecting anything that was already on the screen
				currentUI.copyButtons(&solitaireTakeOptions);
				clearSolitaireButtons(); //rectangle that covers only these new elements instead of clearing the whole screen
				currentUI.renderUI(); //draw the UI
				function = readInput(&currentUI);
				if (function != -1) { //once an option actually entered,
					clearSolitaireButtons();
					rect(stackCoords[undoPlace - 1][0], stackCoords[undoPlace - 1][1], 5, 23, "whiteText"); //draw over just the stack that has been taken from
					goto buttonFunctions; //takes the function number back to the main game loop
				}
			}
			break;
		case 11: //only taking one card from a stack
			drawnCard = stacks[undoPlace]->drawTopCard();
			playerHand.placeCardAtTop(drawnCard);
			break;
			//cases 11 and 12 only get called from within case 10 and the relevant screen clears are called within that
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
			clearSolitaireButtons();
			clearSolitaireHand();
			if (undoPlace == 11) {
				drawnCard = playerHand.drawTopCard();
				spades.placeCardAtTop(drawnCard);
				spades.stack(63, 19);
			}
			else if (undoPlace == 10) {
				drawnCard = playerHand.drawTopCard();
				hearts.placeCardAtTop(drawnCard);
				hearts.stack(63, 13);
			}
			else if (undoPlace == 9) {
				drawnCard = playerHand.drawTopCard();
				clubs.placeCardAtTop(drawnCard);
				clubs.stack(63, 7);
			}
			else if (undoPlace == 8) {
				drawnCard = playerHand.drawTopCard();
				diamonds.placeCardAtTop(drawnCard);
				diamonds.stack(63, 1);
			}
			else if (8 > undoPlace && undoPlace > 0) {
				takeableCards = playerHand.getCards().size();
				for (int i = 0; i < takeableCards; i++) {
					rect(stackCoords[undoPlace - 1][0], stackCoords[undoPlace - 1][1], 5, 23, "whiteText"); //clears the rendering of the stack the cards are being returned to
					drawnCard = playerHand.drawTopCard();
					stacks[undoPlace]->placeCardAtTop(drawnCard);
				}
			}
			else {
				drawnCard = playerHand.drawTopCard();
				drawnDeck.placeCardAtTop(drawnCard);
			}
			coords(0, 36);
			cout << undoPlace;
			break;
		case 14: //place all your held cards onto a stack
			takeableCards = playerHand.getCards().size();
			for (int i = 0; i < takeableCards; i++) { //Try to place the top card n times.
				//Done this way so that if the top card is incompatible it will get put back in your hand and will not be able to check the next one
				//Because checking the next one could skip cards and allow players to cheat.
				drawnCard = playerHand.drawTopCard();
				if (drawnCard.getValue() == "K") { //if the card to be placed is a king
					if (stacks[currentUI.getSelectionVal()[0] + 1]->getCards().size() == 0) { //if there is no stack at attempted position
						stacks[currentUI.getSelectionVal()[0] + 1]->placeCardAtTop(drawnCard);
						rect(stackCoords[currentUI.getSelectionVal()[0]][0], stackCoords[currentUI.getSelectionVal()[0]][1], 5, 23, "whiteText");
						clearSolitaireHand();
						clearSolitaireButtons();
					}
					else {
						playerHand.placeCardAtTop(drawnCard); //Cancel
					}
				}
				else if (stacks[currentUI.getSelectionVal()[0] + 1]->getCards().size() > 0){ //if there's a stack at the attempted position
					compCard = stacks[currentUI.getSelectionVal()[0] + 1]->getCards()[stacks[currentUI.getSelectionVal()[0] + 1]->getCards().size() - 1]; //get the card that you are attempting to put your card on top of
					if (compCard.getFace() == true){ //if the stack has a face-up top card
						if (solitaireValues[drawnCard.getValue()] == solitaireValues[compCard.getValue()] - 1) { //if the card to place is one less than the card to be placed on
							if (drawnCard.getColour() != compCard.getColour()) { //and their colours don't match
								stacks[currentUI.getSelectionVal()[0] + 1]->placeCardAtTop(drawnCard); //place the top card from the player hand
								rect(stackCoords[currentUI.getSelectionVal()[0]][0], stackCoords[currentUI.getSelectionVal()[0]][1], 5, 23, "whiteText");
								clearSolitaireHand();
								clearSolitaireButtons();
							}
							else {
								playerHand.placeCardAtTop(drawnCard); //cancel if wrong colour
							}
						}
						else {
							playerHand.placeCardAtTop(drawnCard); //cancel if wrong number
						}
					}
					else {
						playerHand.placeCardAtTop(drawnCard); //cancel if unavailable stack
					}
				}
				else {
					playerHand.placeCardAtTop(drawnCard); //cancel if no stack
				}
			}
			if (undoPlace > 0 && stacks[undoPlace]->getCards().size() > 0 && playerHand.getCards().size() == 0) { //once your cards have been placed 
				stacks[undoPlace]->flipSpecific(stacks[undoPlace]->getCards().size() - 1, true); //reveal the next card from the stack that it was taken from
			}
			break;
		case 15: //sort your held card into its suit stack
			if (playerHand.getCards().size() == 1) { //only allow when the player only has one card
				drawnCard = playerHand.drawTopCard();
				SetConsoleTextAttribute(hConsole, colours["whiteText"]);
				if (drawnCard.getSuit() == "diamonds") { //if your card is a diamond
					if (diamonds.getCards().size() == 0) { //if the diamonds are empty
						if (drawnCard.getValue() == "A") { //and yours is an ace
							diamonds.placeCardAtTop(drawnCard); //start the diamonds stack
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else { //if yours isn't an ace
							playerHand.placeCardAtTop(drawnCard); //put it back in your hand
							break; //skip the rest of this function
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[diamonds.getCards()[diamonds.getCards().size() - 1].getValue()]) { //if yours is 1 above the current top sorted
							diamonds.placeCardAtTop(drawnCard); //add yours on top
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else { //if it isn't the next value
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					diamonds.stack(63, 1);
				}
				else if (drawnCard.getSuit() == "clubs") { //operates the same as the diamonds
					if (clubs.getCards().size() == 0) {
						if (drawnCard.getValue() == "A") {
							clubs.placeCardAtTop(drawnCard);
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[clubs.getCards()[clubs.getCards().size() - 1].getValue()]) {
							clubs.placeCardAtTop(drawnCard);
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					clubs.stack(63, 7);
				}
				else if (drawnCard.getSuit() == "hearts") {
					if (hearts.getCards().size() == 0) {
						if (drawnCard.getValue() == "A") {
							hearts.placeCardAtTop(drawnCard);
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[hearts.getCards()[hearts.getCards().size() - 1].getValue()]) {
							hearts.placeCardAtTop(drawnCard);
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					hearts.stack(63, 13);
				}
				else if (drawnCard.getSuit() == "spades") {
					if (spades.getCards().size() == 0) {
						if (drawnCard.getValue() == "A") {
							spades.placeCardAtTop(drawnCard);
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					else {
						if (solitaireValues[drawnCard.getValue()] - 1 == solitaireValues[spades.getCards()[spades.getCards().size() - 1].getValue()]) {
							spades.placeCardAtTop(drawnCard);
							clearSolitaireHand();
							clearSolitaireButtons();
						}
						else {
							playerHand.placeCardAtTop(drawnCard);
							break;
						}
					}
					spades.stack(63, 19);
				}
				//if any successful placement was made
				if (8 > undoPlace && undoPlace > 0 && stacks[undoPlace]->getCards().size() > 0) {
					stacks[undoPlace]->flipSpecific(stacks[undoPlace]->getCards().size() - 1, true); //show the next card on the stack where the sorted card was originally
				}
			}
			break;
		case 17: //set blackjack to use 1 deck
			decks = 1;
			standardDeck.empty();
			for (int i = 0; i < decks; i++) {
				standardDeck.copyAll(&resetDeck);
			}
			break;
		case 18: //set blackjack to use 2 decks
			decks = 2;
			standardDeck.empty();
			for (int i = 0; i < decks; i++) {
				standardDeck.copyAll(&resetDeck);
			}
			break;
		case 19: //set blackjack to use 4 decks
			decks = 4;
			standardDeck.empty();
			for (int i = 0; i < decks; i++) {
				standardDeck.copyAll(&resetDeck);
			}
			break;
		case 20: //sets blackjack to solo mode
			groupType = "solo";
			break;
		case 21: //sets blackjack to group mode
			groupType = "group";
			break;
		case 22: //enters the blackjack setup scene
			if (standardDeck.getCards().size() < 12) { //check if there are enough cards left to play
				standardDeck.empty(); //if there aren't, remove the remaining
				for (int i = 0; i < decks; i++) { //and bring in new cards to the player decided deck count
					standardDeck.copyAll(&resetDeck);
				}
			}
			if (groupType == "group") {
				gamemode = 1;
			}
			else {
				gamemode = 7;
			}
			break;
		case 23: //sets Solitaire difficulty
			difficulty = 1;
			break;
		case 24: //sets solitaire difficulty
			difficulty = 3;
			break;
		case 25: //enters solitaire
			gamemode = 3;
			break;
		case 26: //solitaire taking from the sorted cards
			//int which = currentUI.getSelectionVal()[1];
			clearSolitaireButtons();
			switch (currentUI.getSelectionVal()[1]) {
			case 1:
				if (diamonds.getCards().size() > 0) {
					drawnCard = diamonds.drawTopCard();
					playerHand.placeCardAtTop(drawnCard);
					undoPlace = 8;
				}
				break;
			case 2:
				if (clubs.getCards().size() > 0) {
					drawnCard = clubs.drawTopCard();
					playerHand.placeCardAtTop(drawnCard);
					undoPlace = 9;
				}
				break;
			case 3:
				if (hearts.getCards().size() > 0) {
					drawnCard = hearts.drawTopCard();
					playerHand.placeCardAtTop(drawnCard);
					undoPlace = 10;
				}
				break;
			case 4:
				if (spades.getCards().size() > 0) {
					drawnCard = spades.drawTopCard();
					playerHand.placeCardAtTop(drawnCard);
					undoPlace = 11;
				}
				break;
			}
			break;
		default: //unregistered button
			break;
		}

		GetWindowRect(console, &r);
		if (r.top + 727 != r.bottom || r.left + 1283 != r.right) {
			clearScreen();
			MoveWindow(console, r.left, r.top, 1283, 727, TRUE); //forces the window to stay at 720p (maybe a bad idea)
		}
		//Sleep(1000);
		//clearScreen();
	}
	return 1;
}
