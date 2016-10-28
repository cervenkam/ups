#ifndef HAND_H
#define HAND_H

#include "card.h"
class Hand{
	public:
		static const unsigned char ms_SIZE = 4;
		void Clear();
		~Hand();
		bool Add(const Card* card);
		unsigned char Size() const;
		const Card* Get(unsigned char index) const;//NON VALID GETTER
		const Card* Use(unsigned char index);//NON VALID GETTER
	private:
		const Card* m_cards[ms_SIZE];
		unsigned char m_count = 0;
};

#endif
