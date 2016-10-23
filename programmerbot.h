#ifndef	PROGRAMMERBOT_H
#define PROGRAMMERBOT_H

/*
	Bot programmed as I would play
	Author: Martin Cervenka
	Version: 10.04.2016
*/
#include "algorithm.h"
class ProgrammerBot: public Algorithm{
	public:
		/*
			Programmer bot constructor
				=> player Name of the player
				=> ch ID of this player
		*/
		ProgrammerBot(const char* player,unsigned char ch);
		/*
			Copy constructor
		*/
		ProgrammerBot(const Algorithm& algo);
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
		/*
			Selects if this player wants to start new game
				<= 1 want to, -1 dont want to, 0 don't care
		*/
		char Vote();
		/*
			Destructor
		*/
		~ProgrammerBot();
	private:
		/* X cards counter */
		unsigned m_counterX = 0;
		/* E cards counter */
		unsigned m_counterE = 0;
		/* 7 cards counter */
		unsigned m_counter7 = 0;
};

#endif
