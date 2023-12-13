#include "deck.h"
#include "globalFunctions.h"
using namespace std;

deck::deck(vector<card> cards) {
	this->cards = cards;
}

extern deck standardDeck = deck(
	vector<card>{
		card("diamonds", "A", "red", 0, 0), card("diamonds", "2", "red", 0, 0), card("diamonds", "3", "red", 0, 0), card("diamonds", "4", "red", 0, 0), card("diamonds", "4", "red", 0, 0),
		card("diamonds", "5", "red", 0, 0), card("diamonds", "6", "red", 0, 0), card("diamonds", "7", "red", 0, 0), card("diamonds", "8", "red", 0, 0), card("diamonds", "9", "red", 0, 0),
		card("diamonds", "X", "red", 0, 0), card("diamonds", "J", "red", 0, 0), card("diamonds", "Q", "red", 0, 0), card("diamonds", "K", "red", 0, 0),
		card("clubs", "A", "black", 0, 0), card("clubs", "2", "black", 0, 0), card("clubs", "3", "black", 0, 0), card("clubs", "4", "black", 0, 0), card("clubs", "4", "black", 0, 0),
		card("clubs", "5", "black", 0, 0), card("clubs", "6", "black", 0, 0), card("clubs", "7", "black", 0, 0), card("clubs", "8", "black", 0, 0), card("clubs", "9", "black", 0, 0),
		card("clubs", "X", "black", 0, 0), card("clubs", "J", "black", 0, 0), card("clubs", "Q", "black", 0, 0), card("clubs", "K", "black", 0, 0),
		card("hearts", "A", "red", 0, 0), card("hearts", "2", "red", 0, 0), card("hearts", "3", "red", 0, 0), card("hearts", "4", "red", 0, 0), card("hearts", "4", "red", 0, 0),
		card("hearts", "5", "red", 0, 0), card("hearts", "6", "red", 0, 0), card("hearts", "7", "red", 0, 0), card("hearts", "8", "red", 0, 0), card("hearts", "9", "red", 0, 0),
		card("hearts", "X", "red", 0, 0), card("hearts", "J", "red", 0, 0), card("hearts", "Q", "red", 0, 0), card("hearts", "K", "red", 0, 0),
		card("spades", "A", "black", 0, 0), card("spades", "2", "black", 0, 0), card("spades", "3", "black", 0, 0), card("spades", "4", "black", 0, 0), card("spades", "4", "black", 0, 0),
		card("spades", "5", "black", 0, 0), card("spades", "6", "black", 0, 0), card("spades", "7", "black", 0, 0), card("spades", "8", "black", 0, 0), card("spades", "9", "black", 0, 0),
		card("spades", "X", "black", 0, 0), card("spades", "J", "black", 0, 0), card("spades", "Q", "black", 0, 0), card("spades", "K", "black", 0, 0)
});