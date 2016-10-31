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
		void Used(const Card*,unsigned char);
		const Card* Play(bool force) const;//NON VALID GETTER
		char Vote() const;
		void NewHand() const;
};

#endif
