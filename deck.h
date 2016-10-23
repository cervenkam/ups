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
		void Push(Card* card);//GETTER
		~Deck();
		//GETTERS
		Card* Pop();//GETTER
		Card* Peek();//GETTER
		unsigned char Size();//GETTER
	private:
		Card* m_cards[ms_COUNT];
		unsigned char m_count = 0;
};

#endif
