#ifndef DECK_H
#define DECK_H

#include "CardNode.h"

class Deck {
private:
	CardNode *deck;
	CardNode *discard;
	CardNode *hand;
	int count;
	const int NUMVALUES = 13;
	const int NUMSUITS = 4;
	const int HANDSIZE = 5;
public:
	Deck();
	~Deck();
	void destroy(CardNode *node);
	CardNode* removeFromDeck(CardNode *node);
	void addToHand(CardNode *node);
	Card* deal();
	void discardHand();
	Card draw();
	bool Deck::swap(Card toDeck, Card toHand);
	void print(CardNode *list);		// for debugging
	std::string toString();
	void shuffle();
	int getCount() { return count; }
};

#endif