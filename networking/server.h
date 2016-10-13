#ifndef _SERVER_H_
#define _SERVER_H_

#include "commands.h"
#include "commonnetwork.h"
#include "../game.h"
#include <vector>

class Commands;
class Server{
	public:
		void Start();
		char* Receive(int sock);
		Server(unsigned port);
		~Server();
		void Send(int sock,const char* message);
		unsigned GetCountOfGames();
		Game* GetGame(unsigned index);
		void AddGame(Game* game);
		unsigned GetCountOfCommands();
		Commands* GetCommands(unsigned index);
		void AddCommands(Commands* command);
	private:
		unsigned port;
		char internal_storage[MAX_LEN];
		vector<Commands*> commands;
		vector<Game*> games;
};

#endif
