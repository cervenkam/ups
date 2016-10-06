#ifndef _NETWORK_PLAYER_H_
#define _NETWORK_PLAYER_H_

#include "algorithm.h"
#include "networking/server.h"

#define COMMANDS 4

class NetworkPlayer: public Algorithm{
	typedef void (NetworkPlayer::*funcptr)(char*);
	public:
		NetworkPlayer(const char* player,unsigned char ch);
		void SetSocket(int sock);
		void SetServer(Server* server);
		~NetworkPlayer();
		void Call(char* command);
		void Used(Card*,unsigned char);
		Card* Play(bool force);
	private:
		void  BadCommand(char* command);
		void       Login(char* command);
		void    SendCard(char* command);
		void  Disconnect(char* command);  
		void     MyCards(char* message);
		Server* server;
		int sock;
		Card* card_to_play;
		static funcptr commands[COMMANDS];
		static const char* texts[COMMANDS];
		static const char* ranks[8];
		static const char* colors[4];
};


#endif
