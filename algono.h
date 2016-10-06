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
		/*
			Simple bot constructor
				=> ch ID of this player
				=> player Name of the player
		*/
		AlgoNo(const char* player,unsigned char ch);
		/*
			Is called when a card is used (with any player)
				=> card Player card
				=> player ID of the player which played card
		*/
		void Used(Card*,unsigned char);
		/*
			Defines the card which this player will use
				=> force Force the player to play? (he is not lay down the first card)
				<= Card which will player use
		*/
		Card* Play(bool force);
};

#endif
