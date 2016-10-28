#ifndef	PROGRAMMERBOT_H
#define PROGRAMMERBOT_H

#include "algorithm.h"
class ProgrammerBot: public Algorithm{
	public:
		ProgrammerBot(const char* player,unsigned char ch);
		ProgrammerBot(const Algorithm& algo);
		void Used(const Card*,unsigned char);
		~ProgrammerBot();
		const Card* Play(bool force) const;//NON VALID GETTER
		char Vote() const;
	private:
		const Card* GetAnythingElse() const;
		const Card* TryPutFirstCard() const;
		const Card* TryValuableCard() const;
		const Card* GetRandomCard() const;
		const Card* TryByCardCounts(unsigned) const;
		unsigned m_counterX = 0;
		unsigned m_counterE = 0;
		unsigned m_counter7 = 0;
};

#endif
