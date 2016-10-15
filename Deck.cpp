#include "Deck.h"
#include <string>
#include <iostream>
#include "time.h"

using namespace std;

Deck::Deck() {
	deck = new CardNode(Card(), NULL, NULL);
	discard = new CardNode(Card(), NULL, NULL);
	hand = new CardNode(Card(), NULL, NULL);

	// generate cards
	CardNode *node = deck;
	for (int i = 0; i < NUMSUITS; ++i) {
		for (int j = 0; j < NUMVALUES; j++) {
			node->next = new CardNode(Card(j + 2, i), node, NULL);
			node = node->next;
		}
	}

	count = NUMSUITS * NUMVALUES;

	// for "true" random
	srand(time(NULL));
}

Deck::~Deck() {
	destroy(deck);
	destroy(discard);
	destroy(hand);
}

void Deck::destroy(CardNode *head) {
	// delete all nodes
	CardNode *node, *temp;
	node = head;
	while (node != NULL) {
		temp = node;
		node = node->next;
		delete temp;
	}
}

CardNode* Deck::removeFromDeck(CardNode *node) {
	// assuming never removing head node

	//	node->prev <--> node <--> node->next
	//	node->prev <--> node->next

	(node->prev)->next = node->next;
	if (node->next != NULL) {
		(node->next)->prev = node->prev;
	}

	//	node->prev <--> node <--> node->next
	// NULL <-- node --> NULL
	node->prev = NULL;
	node->next = NULL;

	return node;
}

void Deck::addToHand(CardNode *newCard) {
	// insert card to sort hand from lowest to highest card value/suit
	// find node whose ->next has higher card value/suit to append newCard
	CardNode *node = hand;
	while (node->next != NULL) {
		if (newCard->card < (node->next)->card) {
			break;
		}
		node = node->next;
	}

	//	NULL <-- newCard --> NULL
	//	node <-- newCard --> node->next
	newCard->next = node->next;
	newCard->prev = node;

	//	node <--> node->next
	//	node <--> newCard <--> node->next
	if (node->next != NULL) {
		(node->next)->prev = newCard;
	}
	node->next = newCard;
}

Card* Deck::deal() {
	// discard previous hand
	discardHand();

	// draw 5 cards
	for (int i = 0; i < HANDSIZE; ++i) {
		draw();
	}

	// since deck sorts as it inserts one at a time, have to reiterate through hand again to get sorted hand
	Card *arr = new Card[HANDSIZE];
	CardNode *node = hand->next;
	for (int i = 0; i < HANDSIZE; ++i) {
		arr[i] = node->card;
		node = node->next;
	}

	return arr;
}

void Deck::discardHand() {
	// find the last node in hand
	CardNode *tail = hand;
	while (tail->next != NULL) {
		tail = tail->next;
	}

	// hand is empty
	if (tail == hand) {
		return;
	}

	//	discard <--> discard->next
	//	discard <--> hand->next ... tail <--> discard->next

	//	discard <--> discard->next
	//	tail <--> discard->next
	if (discard->next != NULL) {
		(discard->next)->prev = tail;
	}
	tail->next = discard->next;

	//	discard --> discard->next
	//	discard <--> hand->next
	discard->next = hand->next;
	(hand->next)->prev = discard;

	hand->next = NULL;
}

Card Deck::draw() {
	// deck is out of cards, shuffle discard into deck
	if (count == 0) {
		shuffle();
	}

	// take random card
	int random = rand() % count;
	CardNode *node = deck->next;
	for (int i = 0; i < random; ++i) {
		node = node->next;
	}

	// remove card from deck and place into hand
	addToHand(removeFromDeck(node));
	--count;

	return node->card;
}

bool Deck::swap(Card toDeck, Card toHand) {
	// look for card in deck to be swapped into hand
	CardNode *deckNode = deck->next;
	while (deckNode != NULL) {
		if (deckNode->card == toHand) {
			break;
		}
		deckNode = deckNode->next;
	}
	// toHand card is not in deck
	if (deckNode == NULL) {
		return false;
	}

	// look for card in hand to be swapped into deck
	CardNode *handNode = hand->next;
	while (handNode != NULL) {
		if (handNode->card == toDeck) {
			break;
		}
		handNode = handNode->next;
	}
	// toDeck card is not in hand
	if (handNode == NULL) {
		return false;
	}

	// swapping cards
	Card temp = deckNode->card;
	deckNode->card = handNode->card;
	handNode->card = temp;
	return true;
}

void Deck::print(CardNode *list) {
	CardNode *node = list->next;
	cout << "test: " << endl;
	while (node != NULL) {
		cout << node->card.toString() << " ";
		node = node->next;
	}
	cout << endl;
}

string Deck::toString() {
	// return string of all cards in deck
	string str = "";
	CardNode *node = deck->next;
	if (node == NULL) {
		return "";
	}
	str += node->card.toString();
	node = node->next;
	while (node != NULL) {
		str += ", ";
		str += node->card.toString();
		node = node->next;
	}
	return str;
}

void Deck::shuffle() {
	// switch deck and discard == putting discard back into deck
	CardNode *temp = deck;
	deck = discard;
	discard = temp;
	
	// number of cards in hand
	int cardsInHand = 1;
	temp = hand->next;
	while (temp != NULL) {
		temp = temp->next;
		++cardsInHand;
	}

	// remember to subtract cards from current hand
	count = NUMSUITS * NUMVALUES - count - cardsInHand;
}