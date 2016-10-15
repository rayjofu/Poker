#ifndef CARD_H
#define CARD_H

#include <string>

struct Card {
	int value;
	int suit;
	Card() {
	}
	Card(int v, int s) {
		value = v;
		suit = s;
	}
	std::string toString() {
		std::string str = "";
		switch (value) {
		case 11:
			str += "Jack";
			break;
		case 12:
			str += "Queen";
			break;
		case 13:
			str += "King";
			break;
		case 14:
			str += "Ace";
			break;
		default:
			str += std::to_string(value);
		}

		str += " of ";
		
		switch (suit) {
		case 0:
			str += "Clubs";
			break;
		case 1:
			str += "Diamonds";
			break;
		case 2:
			str += "Hearts";
			break;
		case 3:
			str += "Spades";
			break;
		}
		return str;
	}
	bool operator<(const Card &card) {
		if (value < card.value) {
			return true;
		}
		if (value > card.value) {
			return false;
		}
		// rank == card.rank
		return suit < card.suit;
	}
	bool operator==(const Card &card) {
		return value == card.value && suit == card.suit;
	}
};

#endif