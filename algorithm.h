#ifndef ALGORITHM_H
#define ALGORITHM_H

/*
	Abstract algorithm
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "card.h"
#include "hand.h"
#include <vector>
using namespace std;
class Algorithm{
	public:
		/* Static list of all algorithms in the play */
		static vector<Algorithm*> algos;
		/*
			Defines the card which this player will use
				=> force Forces the player to play
				<= Card which will player use
		*/
		virtual Card* Play(bool force) = 0;
		/*
			Abstract algorithm constructor
				=> player Name of the player
				=> ch ID of this player
		*/
		Algorithm(const char* player,unsigned char ch);
		/*
			Lays down the card from player to table
				=> card Played card
		*/
		bool Send(Card* card);	
		/*
			Returns the card which shoud another player react on
				<= Card to be reacted on
		*/
		static Card* FirstCard();
		/*
			Sets the card which should another player react on
				=> card Card to be reacted on
		*/
		static void SetFirstCard(Card* card);
		/*
			Returns the player's hand
				<= Player's card 
		*/
		Hand* GetHand();
		/*
			Adds points to this player
				=> points Points to add
		*/
		void AddPoints(unsigned char points);
		/*
			Gets points of this player
				<= Points of this player
		*/
		unsigned char GetPoints();
		/*
			Clears points of this player
		*/
		void ClearPoints();
		/*
			Identifies the algorithm
		*/
		void Identify();
		/* Name of this player */
		const char* player = "player";
#ifndef TESTING
	protected:
#endif
		/*
			Is called when a card is used (with any player)
				=> card Player card
				=> player ID of the player which played card
		*/
		virtual void Used(Card* card,unsigned char player) = 0;
		/* Defines ID of this player */
		unsigned char myID;
		/* Name of this algoritm */
		const char* name = "algorithm";
#ifndef TESTING
	private:
#endif
		/* Card which should another player react on */
		static Card* first;
		/* Player's hand */
		Hand* hand;
		/* Points of this player */
		unsigned char points = 0;
};

#endif
