#ifndef DECK_H
#define DECK_H

#include <functional>
#include "card.h"
class Deck{
	public:
		static const unsigned char ms_COUNT = 32; //HACK do not change
		void Shuffle();
		void Fill();
		void Print();
		void Free();
		void ThrowAway(unsigned char count);
		void Push(const Card* card);
		~Deck();
		const Card* Pop();
		const Card* Peek();
		unsigned char Size();
		void ForEach(std::function<void (const Card*)>) const;
	private:
		const Card* m_cards[ms_COUNT];
		unsigned char m_count = 0;
};

#endif
