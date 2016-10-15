#ifndef CARDNODE_H
#define CARDNODE_H

#include "Card.h"

struct CardNode {
	Card card;
	CardNode *prev;
	CardNode *next;
	CardNode(Card c, CardNode *p, CardNode *n) {
		card = c;
		prev = p;
		next = n;
	}
};

#endif