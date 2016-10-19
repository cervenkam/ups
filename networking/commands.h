#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "server.h"
#include "../networkplayer.h"
#include <thread>
#include "../game.h"

#define COMMANDS 8

class Server;
class NetworkPlayer;
class Commands{
	typedef void (Commands::*funcptr)(char*);
	public:
		Commands();
		~Commands();
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
		void SetThread(thread*);
		thread* GetThread();
	private:
		void      BadCommand(const char*);
		void      CreateGame(char*);
		void           Login(char*);
		void        SendCard(char*);
		void      Disconnect(char*);  
		void         MyCards(char*);
		void         Welcome(char*);
		void GetCountOfCards(char*);
		void     SendMessage(char*);

		Server* m_server;
		int m_sock;
		NetworkPlayer* m_player;
		Game* m_game;
		Card* m_card_to_play;
		thread* m_thread;

		static funcptr ms_commands[COMMANDS];
		static const char* ms_texts[COMMANDS];
};


#endif
