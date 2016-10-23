#ifndef _NETWORK_PLAYER_H_
#define _NETWORK_PLAYER_H_

/*
	Network player algorithm
	Author: Martin Cervenka
	Version: 06.10.2016
*/
#include "algorithm.h"
#include "programmerbot.h"
#include "networking/commands.h"
class Commands;
class NetworkPlayer: public Algorithm{
	public:
		/*
			Network player constructor
				=> ch ID of this player
				=> player Name of the player
		*/
		NetworkPlayer(const char* player,unsigned char ch);
		/*
			Destroys this player
		*/
		~NetworkPlayer();
		/*
			Sets the game for me and my bot
				=> Game in which this algorithm acts
		*/
		void SetGameForBothMeAndBot(Game* game);
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
			Sets the commands for this player
				=> commands Commands for player
		*/
		void SetCommands(Commands* commands);
		/*
			Gets the commands from this player
				<= commands Commands from player
		*/
		Commands* GetCommands();
		/*
			Sets the card which should player use
		*/
		void SetCard(Card* card);
		/*
			Sets the vote for this player
				=> vote
		*/
		void SetVote(char vote);
		/*
			Selects if this player wants to start new game
				<= 1 want to, -1 dont want to, 0 don't care
		*/
		char Vote();
	private:
		Commands* m_commands;
		Card* m_card;
		char m_vote;
		/* Plays when player is disconnected */
		ProgrammerBot* m_bot;
};

#endif
