#ifndef DECK_H
#define DECK_H

/*
	Class reprezenting cards deck
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "card.h"
class Deck{
	public:
		/* Count of the card */
		static const unsigned char COUNT = 32; //HACK do not change
		/*
			Shuffles this deck
		*/
		void Shuffle();
		/*
			Fills the deck with the cards (in order)
		*/
		void Fill();
		/*
			Prints this deck
		*/
		void Print();
		/*
			Pops the first card from this deck
			Removes the card
				<= First card on this deck
		*/
		Card* Pop();
		/*
			Looks on the first card from this deck
			Does not remove the card
				<= First card on this deck
		*/
		Card* Peek();
		/*
			Push the card to the deck
				=> card Card to be pushed
				<= Is successfully pushed?
		*/
		bool Push(Card* card);
		/*
			Returns size of this deck
				<= Size of this deck
		*/
		unsigned char Size();
		/*
			Throw away cards because of 3, 5 etc. number of players
				=> count Count of the card which will be throwed away from this deck
		*/
		void ThrowAway(unsigned char count);
#ifndef TESTING
	private:
#endif
		/* Array of cards */
		Card* cards[COUNT];
		/* Count of cards in hand */
		unsigned char count = 0;
};

#endif
