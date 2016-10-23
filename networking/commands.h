#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "server.h"
#include "../networkplayer.h"
#include "../semaphore.h"
#include <thread>
#include "../game.h"

#define COMMANDS 9

class Server;
class NetworkPlayer;
class Commands{
	typedef void (Commands::*funcptr)(char*);
	public:
		Commands();
		~Commands();
		Commands(int,Server*,NetworkPlayer*,Game*);
		void SetSocket(int);
		void SetServer(Server*);
		void SetPlayer(NetworkPlayer*);
		void SetGame(Game*);
		void Call(char* command);
		void Start();
		void SetThread(thread*);
		void SetConnected(bool);
		void SetRunning(bool);
		//GETTERS
		Semaphore* GetSemaphore();//GETTER
		thread* GetThread();//GETTER
		bool IsConnected();//GETTER
		bool IsRunning();//GETTER
		int GetSocket();//GETTER
		Server* GetServer();//GETTER
		NetworkPlayer* GetPlayer();//GETTER
		Game* GetGame();//GETTER

		void      Disconnect(char*);  
		void  DisconnectHard(char*);  
	private:
		void      BadCommand(const char*);
		void      CreateGame(char*);
		void           Login(char*);
		void            Vote(char*);
		void        SendCard(char*);
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
		bool m_is_connected = true;
		bool m_running = true;
		Semaphore* m_semaphore;

		static funcptr ms_commands[COMMANDS];
		static const char* ms_texts[COMMANDS];
};


#endif
