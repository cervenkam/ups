#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "server.h"
#include "../networkplayer.h"
#include "../game.h"

#define COMMANDS 7

class Server;
class NetworkPlayer;
class Commands{
	typedef void (Commands::*funcptr)(char*);
	public:
		Commands();
		Commands(int,Server*,NetworkPlayer*,Game*);
		int GetSocket();
		Server* GetServer();
		NetworkPlayer* GetPlayer();
		Game* GetGame();
		void SetSocket(int);
		void SetServer(Server*);
		void SetPlayer(NetworkPlayer*);
		void SetGame(Game*);
		void Call(char* command);
		void Start();
	private:
		void      BadCommand(const char*);
		void      CreateGame(char*);
		void           Login(char*);
		void        SendCard(char*);
		void      Disconnect(char*);  
		void         MyCards(char*);
		void         Welcome(char*);
		void GetCountOfCards(char*);

		Server* server;
		int sock;
		NetworkPlayer* player;
		Game* game;
		Card* card_to_play;

		static funcptr commands[COMMANDS];
		static const char* texts[COMMANDS];
};


#endif
