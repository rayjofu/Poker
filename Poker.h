#ifndef POKER_H
#define POKER_H

#include "Deck.h"

class Poker {
private:
	const int INITIALCASH = 10;
	const int ANTE = 1;
	const int HANDSIZE = 5;
	Deck *deck;
	Card *hand;
	bool *kept;
	int cash;
	void startRound();
	void prompt();
	void displayHand();
	int process(std::string input);
	void swap();
	void exchange(std::string input);
	void sortHand();
	int evaluateHand();
public:
	Poker();
	~Poker();
};

#endif