#ifndef _SERVER_H_
#define _SERVER_H_

#include "commands.h"
#include "commonnetwork.h"
#include "../game.h"
#include <vector>
#include <thread>
#include "../semaphore.h"

class Commands;
class Server{
	public:
		void Start();
		void Stop();
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
		void GarbageCollector();
		void TidyUp(Commands* commands);
	private:
		void StopGame();

		unsigned m_port;
		char m_internal_storage[MAX_LEN];
		vector<Commands*> m_commands;
		vector<Game*> m_games;
		thread* m_garbage_collector;
		Semaphore* m_semaphore;
		Commands* m_cmds;
		int m_sock;
};

#endif
