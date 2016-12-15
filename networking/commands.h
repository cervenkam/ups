#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "server.h"
#include "../networkplayer.h"
#include <thread>

#define COMMANDS 10

class Server;
class NetworkPlayer;
class Commands{
	typedef void (Commands::*funcptr)(const char* const);
	public:
		Commands();
		~Commands();
		Commands(int,Server* const,NetworkPlayer* const,Game* const);
		void SetSocket(int);
		void SetServer(Server* const);
		void SetPlayer(NetworkPlayer* const);
		void SetGame(Game* const);
		void Start();
		void SetThread(thread* const);
		void SetConnected(bool);
		void SetRunning(bool);
		bool IsConnected() const;
		bool IsRunning() const;
		int GetSocket() const;
		void GarbageCollect();
		void Join() const;
		void Notify() const;
		const char* GetAlgorithmName(unsigned);

		bool     TrySend(Commands*,const char* const) const;
		bool     TrySend(int      ,const char* const) const;
		char* TryReceive(int sock                   ) const;
		void             Call(const char* const);
		void       Disconnect(const char* const);  
		void          MyCards(const char* const) const;
		void  GetCountOfCards(const char* const) const;
		void            Table(const char* const) const;
	private:
		void            Login(const char* const);
		void         SendCard(const char* const);
		void      SendMessage(const char* const) const;
		void       BadCommand(const char* const) const;
		void       CreateGame(const char* const) const;
		void             Vote(const char* const) const;
		void          Welcome(const char* const) const;

		int FindMethod(const char* const);
		bool TotalCompare(const char* const,int);
		void InitPlayer(Game*);
		void FindPlayerInGame(Game*,const char* const);
		void TryStartMyGame();
		static unsigned NumberOfSameLetters(const char* const,const char* const,unsigned);
		void StopGame();
		bool FindReasonToStay(unsigned);
		thread* GetThread() const;
		Semaphore* GetSemaphore() const;
		Server* GetServer() const;
		NetworkPlayer* GetPlayer() const;
		Game* GetGame() const;

		Server* m_server;
		int m_sock;
		NetworkPlayer* m_player;
		Game* m_game;
		const Card* m_card_to_play;
		thread* m_thread;
		bool m_is_connected = true;
		bool m_running = true;
		Semaphore* m_semaphore;

		static Commands* ms_cmds;
		static funcptr ms_commands[COMMANDS];
		static const char* const ms_texts[COMMANDS];

		void     _SendMessage(const char* const);
		void      _BadCommand(const char* const);
		void      _CreateGame(const char* const);
		void            _Vote(const char* const);
		void           _Table(const char* const);
		void         _MyCards(const char* const);
		void         _Welcome(const char* const);
		void _GetCountOfCards(const char* const);
};


#endif
