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
		void Used(const Card*,unsigned char);
		void SetCommands(Commands* commands);
		void SetCard(const Card* card);
		void SetVote(char vote);
		char Vote() const;
		const Card* Play(bool force) const;//NON VALID GETTER
		void TrySend(char* buff);
		void SetNextCard(unsigned,unsigned);
	private:
		Commands* GetCommands();
		Commands* m_commands;
		const Card* m_card;
		char m_vote;
		ProgrammerBot* m_bot;
};

#endif
