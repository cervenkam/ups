#ifndef _NETWORK_PLAYER_H_
#define _NETWORK_PLAYER_H_

#include "algorithm.h"
#include "programmerbot.h"
#include "networking/commands.h"
class Commands;
class NetworkPlayer: public Algorithm{
	public:
		NetworkPlayer(const char* player,unsigned char ch);
		~NetworkPlayer();
		void SetGameForBothMeAndBot(Game* game);
		void Used(Card*,unsigned char);
		void SetCommands(Commands* commands);
		void SetCard(Card* card);
		void SetVote(char vote);
		//GETTERS
		char Vote();//GETTER
		Commands* GetCommands();//GETTER
		Card* Play(bool force);//GETTER
	private:
		Commands* m_commands;
		Card* m_card;
		char m_vote;
		ProgrammerBot* m_bot;
};

#endif
