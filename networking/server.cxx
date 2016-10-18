#include "server.h"
#include "../stdmcr.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <thread>
#include "commonnetwork.h"

using namespace std;
Server::Server(unsigned port){
	m_port = port;
}

Server::~Server(){
	unsigned len = m_commands.size();
	for(unsigned a=0; a<len; a++){
		delete m_commands[a];
	}
	len = m_games.size();
	for(unsigned a=0; a<len; a++){
		delete m_games[a];
	}
}

void Server::Start(){
	int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
	TEST_ERR(sock<0,"Socket not created")
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	saddrin addr, in_addr;
	fill_saddrin_any(addr,m_port);
	int bnd = bind(sock,reinterpret_cast<saddr*>(&addr),sizeof(saddrin));
	TEST_ERR(bnd<0,"Socket not binded")
	int lsn = listen(sock, 10);
	TEST_ERR(lsn<0,"Cannot listen")
	STDMSG("0;36","Listen:     port " << m_port);
	while(1){
		unsigned len = sizeof(saddrin);
		errno = 0;
		int sckt = accept(sock,reinterpret_cast<saddr*>(&in_addr),&len);
		TEST_ERR(errno>0,strerror(errno));
		TEST_ERR_DO(sckt<0,"Cannot accept socket",close(sock))
		STDMSG("0;36","Connection: " << inet_ntoa(in_addr.sin_addr) << ":" << ntohs(in_addr.sin_port));
		Commands* cmd = new Commands();
		cmd->SetServer(this);
		cmd->SetSocket(sckt);
		m_commands.push_back(cmd);
		thread thr(&Commands::Start,cmd);
		thr.detach();
	}
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
