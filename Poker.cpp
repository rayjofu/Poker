#include "Poker.h"
#include <iostream>
#include <string>

using namespace std;

Poker::Poker() {
	deck = new Deck();
	hand = NULL;
	kept = new bool[HANDSIZE];
	cash = INITIALCASH;
	cout << "Welcome to Video Poker!" << endl;
	cout << "You have started with $" << cash << endl;

	string input;
	int action;
	int gains;
	while (1) {
		startRound();
		while (1) {
			prompt();
			while (1) {
				getline(cin, input);
				if (!input.empty()) {
					break;
				}
			}
			action = process(input);
			if (action < 0) {
				continue;
			}
			if (action == 1) {
				gains = evaluateHand();
				cash += gains;
				
				cout << "You earned $" << gains << endl;
				cout << "You now have $" << cash << endl << endl;
				break;
			}
			if (action == 2) {
				continue;
			}
			if (action == 0) {
				return;
			}
		}
		cout << "Press enter to play another round...";
		getline(cin, input);
		cout << "--------------------------------------------------" << endl << endl;

		if (cash <= 0) {
			cout << "You are out of money! GAME OVER!" << endl;
			return;
		}
	}
}

Poker::~Poker() {
	delete deck;
	delete[] hand;
	delete[] kept;
}

void Poker::startRound() {
	cout << "You pay a $" << ANTE << " ante and now have $" << --cash << endl << endl;
	if (hand != NULL) {
		delete[] hand;
	}
	hand = deck->deal();
}

void Poker::prompt() {
	displayHand();

	cout << "The deck contains " << deck->getCount() << " cards." << endl;

	cout << endl << "OPTIONS..." << endl;
	cout << "- Type the letters for the cards you wish to keep. (i.e., \"acd\")" << endl;
	cout << "- Type \"deck\" to view the cards remaining in the deck." << endl;
	cout << "- Type \"none\" to discard all cards in your hand." << endl;
	cout << "- Type \"all\" to keep all cards in your hand." << endl;
	cout << "- Type \"exit\" to exit the game." << endl;
	cout << "- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck." << endl;
	cout << "YOUR CHOICE : ";
}

void Poker::displayHand() {
	cout << "Your hand contains:" << endl;
	for (int i = 0; i < HANDSIZE; ++i) {
		cout << (char)('A' + i) << ": " <<  hand[i].toString() << endl;
	}
	cout << endl;
}

int Poker::process(string input) {
	for (int i = 0; i < input.length(); ++i) {
		input[i] = toupper(input[i]);
	}

	if (input == "DECK") {
		cout << endl << deck->toString() << endl << endl;
		return -1;
	}
	if (input == "NONE") {
		exchange("");
		return 1;
	}
	if (input == "ALL") {
		exchange("ABCDE");
		return 1;
	}
	if (input == "EXIT") {
		return 0;
	}
	if (input == "SWAP") {
		swap();
		return -1;
	}

	for (int i = 0; i < input.length(); ++i) {
		switch (input[i]) {
		case 'A':
			break;
		case 'B':
			break;
		case 'C':
			break;
		case 'D':
			break;
		case 'E':
			break;
		default:
			cout << "Sorry, I didn't understand, please try again..." << endl << endl;
			return -1;
		}
	}
	exchange(input);

	return 1;
}

void Poker::swap() {
	string card;
	cout << "Enter the letter of the card in hand: ";
	getline(cin, card);
	card[0] = toupper(card[0]);
	if (card[0] < 'A' || card[0] > 'E') {
		cout << "Invalid hand card." << endl << endl;
		return;
	}

	int value;
	cout << "Enter the value of the card in the deck to swap with: ";
	cin >> value;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (value == 1) {
		value = 14;
	}
	if (value > 14 || value < 0) {
		cout << "Invalid card value." << endl << endl;
		return;
	}

	string input;
	cout << "Enter the suit (c,d,h,s) of the card in the deck to swap with: ";
	getline(cin, input);
	input[0] = toupper(input[0]);
	if (input[0] != 'C' && input[0] != 'D' && input[0] != 'H' && input[0] != 'S') {
		cout << "Invalid card suit." << endl << endl;
		return;
	}

	int suit;
	switch (input[0]) {
	case 'C':
		suit = 0;
		break;
	case 'D':
		suit = 1;
		break;
	case 'H':
		suit = 2;
		break;
	case 'S':
		suit = 3;
		break;
	}

	Card toHand(value, suit);
	int index = card[0] - 'A';
	if (!deck->swap(hand[index], toHand)) {
		cout << "Invalid deck card." << endl << endl;
		return;
	}

	hand[index] = toHand;
	sortHand();
	cout << endl;
}

void Poker::exchange(string input) {
	int numKept = 0;
	string output = "";
	for (int i = 0; i < HANDSIZE; ++i) {
		kept[i] = false;

		// discard
		if (input.find('A' + i) == string::npos) {
			hand[i] = deck->draw();
		}
		// keep
		else {
			kept[i] = true;
			++numKept;
		}
	}
	sortHand();
	for (int i = 0; i < HANDSIZE; ++i) {
		output += (char)('A' + i);
		output += ": ";
		output += hand[i].toString();
		if (kept[i]) {
			output += " (kept)";
		}
		output += '\n';
	}
	cout << endl << "You kept " << numKept << " and drew " << HANDSIZE - numKept << " cards." << endl;
	cout << "Your hand now contains:" << endl;
	cout << output << endl;
}

void Poker::sortHand() {
	Card tempCard;
	bool tempBool;
	int min = 0;
	for (int i = 0; i < HANDSIZE - 1; ++i) {
		min = i;
		for (int j = i + 1; j < HANDSIZE; ++j) {
			
			if (hand[j] < hand[min]) {
				min = j;
			}
		}
		if (min != i) {
			tempCard = hand[i];
			hand[i] = hand[min];
			hand[min] = tempCard;
			tempBool = kept[i];
			kept[i] = kept[min];
			kept[min] = tempBool;
		}
	}
}

int Poker::evaluateHand() {
	bool flush = false;
	bool straight = false;
	bool triple = false;
	bool pair = false;

	// flush
	if (hand[0].suit == hand[1].suit && hand[1].suit == hand[2].suit && hand[2].suit == hand[3].suit && hand[3].suit == hand[4].suit) {
		flush = true;
	}

	// straight
	if (hand[0].value + 1 == hand[1].value && hand[1].value + 1 == hand[2].value && hand[2].value + 1 == hand[3].value) {
		if (hand[3].value + 1 == hand[4].value) {
			straight = true;
		}
		// 2 3 4 5 A
		else if (hand[3].value == 5 && hand[4].value == 14) {
			straight = true;
		}
	}
	
	if (flush && straight) {
		// royal flush
		if (hand[0].value == 10) {
			cout << "Royal flush!" << endl;
			return 800;
		}
		// (straight flush
		cout << "Straight flush!" << endl;
		return 50;
	}

	// 4 of a kind
	// X X X X _
	if (hand[0].value == hand[1].value && hand[1].value == hand[2].value && hand[2].value == hand[3].value) {
		cout << "Four of a kind!" << endl;
		return 25;
	}
	// _ X X X X
	if (hand[1].value == hand[2].value && hand[2].value == hand[3].value && hand[3].value == hand[4].value) {
		cout << "Four of a kind!" << endl; 
		return 25;
	}

	// X X X ? ?
	if (hand[0].value == hand[1].value && hand[1].value == hand[2].value) {
		// full house
		if (hand[3].value == hand[4].value) {
			cout << "Full house!" << endl;
			return 9;
		}
		// triple
		else {
			triple = true;
		}
	}
	// ? ? X X X
	if (hand[2].value == hand[3].value && hand[3].value == hand[4].value) {
		// full house
		if (hand[0].value == hand[1].value) {
			cout << "Full house!" << endl;
			return 9;
		}
		// triple
		else {
			triple = true;
		}
	}

	// flush
	if (flush) {
		cout << "Flush!" << endl;
		return 6;
	}

	// straight
	if (straight) {
		cout << "Straight!" << endl;
		return 4;
	}

	// triple
	if (triple) {
		cout << "Triple!" << endl;
		return 3;
	}

	// X X ? ? _
	if (hand[0].value == hand[1].value) {
		// 2 pair
		if (hand[2].value == hand[3].value) {
			cout << "Two pair!" << endl;
			return 2;
		}
		// pair
		else if (hand[0].value >= 11) {
			pair = true;
		}
	}

	// _ X X ? ?
	if (hand[1].value == hand[2].value) {
		// 2 pair
		if (hand[3].value == hand[4].value) {
			cout << "Two pair!" << endl;
			return 2;
		}
		// pair
		else if (hand[1].value >= 11) {
			pair = true;
		}
	}

	// ? ? _ X X
	if (hand[3].value == hand[4].value) {
		// 2 pair
		if (hand[0].value == hand[1].value) {
			cout << "Two pair!" << endl;
			return 2;
		}
		// pair
		else if (hand[3].value >= 11) {
			pair = true;
		}
	}

	// pair
	if (pair) {
		cout << "Pair of Jacks or better!" << endl;
		return 1;
	}

	cout << "No Poker hand scored. :(" << endl;
	return 0;
}