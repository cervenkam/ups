#ifndef GAME_H
#define GAME_H

/*
	Class reprezenting the game
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "deck.h"
#include "algorithm.h"
#include "config.h"
#include <thread>
#include <vector>
class Algorithm;
class Configuration;
class Game{
	public:
		/*
			Creates new game
				=> conf Game configuration
		*/
		Game(Configuration* conf);
		/*
			Destruct the game
		*/
		~Game();
		/*
			Returns the game algorithm
				=> player Player ID
				<= Player algorithm
		*/
		Algorithm* GetAlgorithm(unsigned player);

		/*
			Returns number of players
				<= Number of players
		*/
		unsigned GetCountOfPlayers();

		/*
			Creates voting system for all players to repeat this game
				<= Should be game repeated
		*/
		bool VotePasses();
		/*
			Starts the game
		*/
		void Start();
		/*
			Ends the game
		*/
		void End();
		/*
			Prints the game
		*/
		void Print();
		/*
			Is a player with given ID a starting player of this round?
				=> ID Id of player
				<= Is he?
		*/
		bool IsHeStarted(unsigned char ID);
		/*
			Sets the name of this game
				=> name Name of this game
		*/
		void SetName(char* name);
		/*
			Gets the name of this game
				<= Name of this game
		*/
		char* GetName();
		/*
			Sets the starting player of this round
				=> ID Starting player's ID
		*/
		void SetStarted(unsigned char ID);
		/*
			Returns the starting player of this round
				<= Starting player's ID
		*/
		unsigned char GetStarted();
		/*
			Game in new thread
		*/
		void StartParallel();
		/*
			Stops game in new thread
		*/
		void StopParallel();
		/*
			Returns the card which shoud another player react on
				<= Card to be reacted on
		*/
		Card* FirstCard();
		/*
			Sets the card which should another player react on
				=> card Card to be reacted on
		*/
		void SetFirstCard(Card* card);
	private:
		/*
			Prepares the game
				<= Can we continue?
		*/
		bool Prepare();
		/*
			Does multiple loops
		*/
		void Loop();
		/*
			Choose card from player's hand
				=> player Player who plays
				=> winner Actual leader of this hand
				=> card Choosed card
				=> started Is this player started?
				<= Is end of game?
		*/
		bool ChooseCard(unsigned& player,unsigned& winner,Card*& card, bool& started);
		/*
			Fills all player's hands
				<= Is end of game?
		*/
		bool FillHands();
		/*
			Does one loop in game
				=> winner Actual leader of this hand
				=> player Player who plays
				<= Is end of game?
		*/
		bool OneHand(unsigned& winner, unsigned& player);
		/*
			Uses card which player chose
				=> card Card which player chose
				=> player Player who chose card
				=> winner Actual leader of this hand
				<= Is end of game?
		*/
		bool UseCard(Card* card,unsigned& player, unsigned& winner);
		/*
			Check same cards, if there are full hand of same card, ends the game
				<= Is end of the game
		*/
		bool SameCards();
		/*
			Determines the winner of this hand
				=> card Used card
				=> player Player who used the card
				=> winner Actual winner
		*/
		void DetermineWinner(Card* card,unsigned& player, unsigned& winner);
		/* Card which should another player react on */
		Card* m_first = nullptr;
		/* Count of players in this game */
		unsigned char m_players;
		/* Determines the player which started this round */
		unsigned char m_started = 0;
		/* Deck of not used card */
		Deck* m_deck;
		/* Array of player algorithms */
		Algorithm** m_algos;
		/* Number of points on table */
		unsigned char m_points = 0;
		/* Name of the game */
		char* m_name;
		/* Configuration */
		Configuration* m_conf;
		/* Thread in which this game runs */
		thread* m_thr = nullptr;
		/* is the game running */
		bool m_end_of_game = false;
};

#endif
