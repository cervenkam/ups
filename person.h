#ifndef PERSON_H
#define PERSON_H

#include "algorithm.h"
class Person: public Algorithm{
	public:
		Person(const char* player,unsigned char ch);
		void Used(Card* card,unsigned char player);
		void Print(unsigned card,bool clear);
		//GETTERS
		Card* Play(bool force);//GETTER
		char Vote();//GETTER
};

#endif
