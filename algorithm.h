#ifndef ALGORITHM_H
#define ALGORITHM_H

/*
	Abstract algorithm
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "card.h"
#include "hand.h"
#include "lang.h"
#include <vector>
#include "semaphore.h"
using namespace std;
class Algorithm{
	public:
		/* Static list of all algorithms in the play */
		static vector<Algorithm*> ms_algos;
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
			Copy constructor
		*/
		Algorithm(const Algorithm& algorithm);
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
			Set this player ready
		*/
		void SetReady();
		/*
			Set this player ready
				=> Is player ready
		*/
		void SetReady(bool);
		/*
			Is this player ready to play
		*/
		bool IsReady();
		/*
			Identifies the algorithm
		*/
		void Identify();
		/*
			Gets semapohore for this object
		*/
		Semaphore* GetSemaphore();
		/*
			Virtual destuctor
		*/
		virtual ~Algorithm();
		/* Name of this player */
		const char* m_player;
	protected:
		/*
			Is called when a card is used (with any player)
				=> card Player card
				=> player ID of the player which played card
		*/
		virtual void Used(Card* card,unsigned char player) = 0;
		/* Defines ID of this player */
		unsigned char m_myID;
		/* Name of this algoritm */
		const char* m_name = "algorithm";
	private:
		/* Card which should another player react on */
		static Card* m_first;
		/* Player's hand */
		Hand* m_hand;
		/* Points of this player */
		unsigned char m_points = 0;
		/* Is player ready to start */
		bool m_ready = false;
		/* Semaphore for all algorithms */
		Semaphore* m_semaphore;
		/* Is this algorithm only a copy? */
		bool m_copy = false;
};

#endif
