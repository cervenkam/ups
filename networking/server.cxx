#include "server.h"
#include "../stdmcr.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <thread>
#include "commonnetwork.h"

using namespace std;
Server::Server(unsigned port){
	m_semaphore = new Semaphore(0);
	m_port = port;
}

Server::~Server(){
	unsigned len=m_commands.size();
	for(unsigned a=0; a<len; a++){
		m_commands[a]->SetSocket(0);
		m_commands[a]->GetThread()->join();
		delete m_commands[a];
	}
	len = m_games.size();
	for(unsigned a=0; a<len; a++){
		delete m_games[a];
	}
	delete m_semaphore;
}

void Server::Start(){
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
	TEST_ERR(m_sock<0,"Socket not created")
	int opt = 1;
	setsockopt(m_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	saddrin addr, in_addr;
	fill_saddrin_any(addr,m_port);
	int bnd = bind(m_sock,reinterpret_cast<saddr*>(&addr),sizeof(saddrin));
	TEST_ERR(bnd<0,"Socket not binded")
	int lsn = listen(m_sock, 10);
	TEST_ERR(lsn<0,"Cannot listen")
	STDMSG("0;36","Listen:     port " << m_port);
	m_garbage_collector = new thread(&Server::GarbageCollector,this);
	while(1){
		unsigned len = sizeof(saddrin);
		errno = 0;
		int sckt = accept(m_sock,reinterpret_cast<saddr*>(&in_addr),&len);
		TEST_ERR_DO(errno>0,STDMSG("0;36","Stopping"),break);
		TEST_ERR_DO(sckt<0,"Cannot accept socket",close(m_sock))
		STDMSG("0;36","Connection: " << inet_ntoa(in_addr.sin_addr) << ":" << ntohs(in_addr.sin_port));
		Commands* cmd = new Commands();
		cmd->SetServer(this);
		cmd->SetSocket(sckt);
		thread* thr = new thread(&Commands::Start,cmd);
		cmd->SetThread(thr);
		m_commands.push_back(cmd);
	}
	m_cmds = nullptr;
	m_port = 0;
	m_semaphore->Notify();
	m_garbage_collector->join();
	delete m_garbage_collector;
}
void Server::GarbageCollector(){
	while(m_port){
		m_semaphore->Wait();
		if(m_cmds != nullptr){
			m_cmds->GetThread()->join();
			delete m_cmds->GetThread();
			delete m_cmds;
			STDMSG("0;36","Disconnected");
		}
	}
}
void Server::Stop(){
	close(m_sock);
}
void Server::TidyUp(Commands* cmds){
	m_cmds=cmds;
	m_semaphore->Notify();
}
char* Server::Receive(int sock){
	uint32_t netlen;
	int rcv = recv(sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR_RET(rcv<0,"Cannot get message length",nullptr)
	unsigned length = ntohl(netlen);
	TEST_ERR_RET(length>=MAX_LEN,"Message too long",nullptr)
	rcv = recv(sock,&m_internal_storage,sizeof(char)*MAX_LEN,0);
	TEST_ERR_RET(rcv<0,"Cannot get message",nullptr)
	m_internal_storage[length]='\0';
	STDMSG("1;33","Received:   " << m_internal_storage);
	return m_internal_storage;
}
void Server::Send(int sock,const char* message){
	STDMSG("0;32","Sending:    " << message);
	unsigned length = strlen(message);
	uint32_t netlen = htonl(length);
	int snd = send(sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR(snd<0,"Cannot send message length")
	snd = send(sock,message,sizeof(char)*length,0);
	TEST_ERR(snd<0,"Cannot send message")
}

unsigned Server::GetCountOfGames(){
	return m_games.size();
}

Game* Server::GetGame(unsigned index){
	return m_games[index];
}

void Server::AddGame(Game* game){
	m_games.push_back(game);
}

unsigned Server::GetCountOfCommands(){
	return m_commands.size();
}

Commands* Server::GetCommands(unsigned index){
	return m_commands[index];
}

void Server::AddCommands(Commands* command){
	m_commands.push_back(command);
}
