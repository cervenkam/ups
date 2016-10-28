#ifndef PERSON_H
#define PERSON_H

#include "algorithm.h"
class Person: public Algorithm{
	public:
		Person(const char* player,unsigned char ch);
		void Used(const Card* card,unsigned char player);
		void Print(unsigned card,bool clear) const;
		const Card* Play(bool force) const;//NON VALID GETTER
		char Vote() const;
	private:
		bool OnePlay(const Hand*,unsigned&) const;
};

#endif
