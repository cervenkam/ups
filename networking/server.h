#ifndef _SERVER_H_
#define _SERVER_H_

#include "commands.h"
#include "commonnetwork.h"
#include "../game.h"
#include <thread>

class Commands;
class Server{
	public:
		void Start();
		void Stop();
		char* Receive(int sock) const;
		Server(unsigned port);
		~Server();
		void Send(int sock,const char* message) const;
		void AddGame(Game* game);
		void AddCommands(Commands* command);
		void TidyUp(Commands* commands);
		bool NewConnection(saddrin); 
		unsigned GetCountOfGames() const; 
		bool IsRunning() const;
		void RemoveCommands(Commands*);
		void RemoveGame(Game*);
		void GarbageCollector();
		void SetCommands(Commands*);
		Game* GetGame(unsigned index) const;
	private:
		void StopGame();
		void Listen(saddrin&);
		Commands* GetCommands() const;
		Semaphore* GetGCSemaphore() const;

		unsigned m_port;
		mutable char m_internal_storage[MAX_LEN];
		std::vector<Commands*> m_commands;
		std::vector<Game*> m_games;
		std::thread* m_garbage_collector;
		Commands* m_cmds;
		Semaphore* m_semaphore_gc;
		int m_sock;
};

#endif
