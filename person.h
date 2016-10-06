#ifndef PERSON_H
#define PERSON_H

/*
	Human player "algorithm"
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "algorithm.h"
class Person: public Algorithm{
	public:
		/*
			Creates person player
				=> ch ID of this player
				=> player Name of the player
		*/
		Person(const char* player,unsigned char ch);
		/*
			Proceeds the used card from any player
				=> card Played card
				=> player Player which used card
		*/
		void Used(Card* card,unsigned char player);
		/*
			Chooses the card which will be used
				=> force Forces player to play
				<= Card which will be used
		*/
		Card* Play(bool force);
		void Print(unsigned card,bool clear);
};

#endif
