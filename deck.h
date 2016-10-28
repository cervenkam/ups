#ifndef DECK_H
#define DECK_H

#include "card.h"
class Deck{
	public:
		static const unsigned char ms_COUNT = 32; //HACK do not change
		void Shuffle();
		void Fill();
		void Print();
		void ThrowAway(unsigned char count);
		void Push(Card* card);
		~Deck();
		Card* Pop();//NON VALID GETTER
		Card* Peek();//NON VALID GETTER
		unsigned char Size();
	private:
		Card* m_cards[ms_COUNT];
		unsigned char m_count = 0;
};

#endif
