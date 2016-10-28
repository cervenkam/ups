#ifndef HAND_H
#define HAND_H

#include "card.h"
class Hand{
	public:
		static const unsigned char ms_SIZE = 4;
		void Clear();
		~Hand();
		//GETTERS
		bool Add(Card* card);//GETTER
		unsigned char Size();//GETTER
		Card* Get(unsigned char index);//GETTER
		Card* Use(unsigned char index);//GETTER
	private:
		Card* m_cards[ms_SIZE];
		unsigned char m_count = 0;
};

#endif
