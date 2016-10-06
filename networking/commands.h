#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "server.h"
#include "../networkplayer.h"
#include "../game.h"

#define COMMANDS 6

class Server;
class Commands{
	typedef void (Commands::*funcptr)(char*);
	public:
		Commands();
		Commands(int,Server*,NetworkPlayer*,Game*);
		void SetSocket(int);
		void SetServer(Server*);
		void SetPlayer(NetworkPlayer*);
		void SetGame(Game*);
		void Call(char* command);
		void Start();
	private:
		void  BadCommand(const char*);
		void  CreateGame(char*);
		void       Login(char*);
		void    SendCard(char*);
		void  Disconnect(char*);  
		void     MyCards(char*);
		void        Ping(char*);

		Server* server;
		int sock;
		NetworkPlayer* player;
		Game* game;
		Card* card_to_play;

		static funcptr commands[COMMANDS];
		static const char* texts[COMMANDS];
		static const char* ranks[8];
		static const char* colors[4];
};


#endif
