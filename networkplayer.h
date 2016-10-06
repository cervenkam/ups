#ifndef _NETWORK_PLAYER_H_
#define _NETWORK_PLAYER_H_

/*
	Network player algorithm
	Author: Martin Cervenka
	Version: 06.10.2016
*/
#include "algorithm.h"
class NetworkPlayer: public Algorithm{
	public:
		/*
			Simple bot constructor
				=> ch ID of this player
				=> player Name of the player
		*/
		NetworkPlayer(const char* player,unsigned char ch);
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
