#ifndef	PROGRAMMERBOT_H
#define PROGRAMMERBOT_H

#include "algorithm.h"
class ProgrammerBot: public Algorithm{
	public:
		ProgrammerBot(const char* player,unsigned char ch);
		ProgrammerBot(const Algorithm& algo);
		void Used(Card*,unsigned char);
		~ProgrammerBot();
		//GETTERS
		Card* Play(bool force);//GETTER
		char Vote();//GETTER
	private:
		Card* GetAnythingElse();
		Card* TryPutFirstCard();
		Card* TryValuableCard();
		Card* GetRandomCard();
		Card* TryByCardCounts(unsigned);
		unsigned m_counterX = 0;
		unsigned m_counterE = 0;
		unsigned m_counter7 = 0;
};

#endif
