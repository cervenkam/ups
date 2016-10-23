#ifndef ALGONO_H
#define ALGONO_H

/*
	Simple bot algorithm
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "algorithm.h"
class AlgoNo: public Algorithm{
	public:
		AlgoNo(const char* player,unsigned char ch);
		~AlgoNo();
		//GETTERS
		void Used(Card*,unsigned char);//GETTER
		Card* Play(bool force);//GETTER
		char Vote();//GETTER
};

#endif
