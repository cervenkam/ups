#ifndef HAND_H
#define HAND_H

/*
	Hand (set) of cards
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "card.h"
class Hand{
	public:
		/* Number of cards in hand */
		static const unsigned char SIZE = 4;
		/*
			Sets the card of specified position
			DEPRECATED - do not use this method, use Add instead
				=> position Position to set
				=> card Card which will be pushed on this position
		*/
		bool Set(unsigned char position,Card* card);
		/*
			Returns size of the hand (number of cards)
				<= Number of cards in hand
		*/
		unsigned char Size();
		/*
			Returns the card on specified position
				<= index Specified position
				=> Card on position
		*/
		Card* Get(unsigned char index);
		/*
			Uses the card
				=> index Position of the card in hand
				<= Card to be used
		*/
		Card* Use(unsigned char index);
		/*
			Adds the card to hand
				=> card Card to be added
				<= Has been added?
		*/
		bool Add(Card* card);
		/*
			Clears the hand
		*/
		void Clear();
#ifndef TESTING
	private:
#endif
		/* Cards in hand */
		Card* cards[SIZE];
		/* Count of cards in hand */
		unsigned char count = 0;
};

#endif
