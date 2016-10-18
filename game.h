#ifndef GAME_H
#define GAME_H

/*
	Class reprezenting the game
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "deck.h"
#include "algorithm.h"
class Game{
	public:
		/*
			Creates new game
				=> players Number of players
				=> algos Players algos
		*/
		Game(unsigned char players,Algorithm** algos);
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
		static void SetStarted(unsigned char ID);
		/*
			Returns the starting player of this round
				<= Starting player's ID
		*/
		static unsigned char GetStarted();
		/*
			Game in new thread
		*/
		void StartParallel();
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
		/* Count of players in this game */
		unsigned char m_players;
		/* Determines the player which started this round */
		static unsigned char ms_started;
		/* Deck of not used card */
		Deck* m_deck;
		/* Array of player algorithms */
		Algorithm** m_algos;
		/* Number of points on table */
		unsigned char m_points = 0;
		/* Name of the game */
		char* m_name;
};

#endif
