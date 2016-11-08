#include "server.h"
#include "../stdmcr.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include "commonnetwork.h"

using namespace std;
Server::Server(unsigned port){
	m_port = port;
	m_semaphore_gc = new Semaphore(0);
	STDMSG("1;35","Created:    Server");
}

Server::~Server(){
	unsigned len=m_commands.size();
	for(unsigned a=0; a<len; a++){
		m_commands[a]->SetSocket(0);
		m_commands[a]->Join();
		delete m_commands[a]; //created in Start funtion
	}
	len = m_games.size();
	for(unsigned a=0; a<len; a++){
		delete m_games[a]; //created in Commands::CreateGame
	}
	delete m_semaphore_gc; //created in constructor
	STDMSG("1;35","Deleted:    Server");
}

void Server::Listen(saddrin& addr){
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
	TEST_ERR(m_sock<0,"Socket not created")
	int opt = 1;
	setsockopt(m_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	fill_saddrin_any(addr,m_port);
	int bnd = bind(m_sock,reinterpret_cast<saddr*>(&addr),sizeof(saddrin));
	TEST_ERR(bnd<0,"Socket not binded")
	int lsn = listen(m_sock, 10);
	TEST_ERR(lsn<0,"Cannot listen")
	STDMSG("0;36","Listen:     port " << m_port);
	m_garbage_collector = new thread(&Server::GarbageCollector,this); //deleted at the end of Start function
}
void Server::GarbageCollector(){
	while(IsRunning()){
		GetGCSemaphore()->Wait();
		if(GetCommands()){
			GetCommands()->GarbageCollect();
			delete GetCommands();
			SetCommands(nullptr);
		}
	}
}
void Server::TidyUp(Commands* cmd){
	SetCommands(cmd);
	GetGCSemaphore()->Notify();
}
void Server::SetCommands(Commands* cmd){
	m_cmds = cmd;
}
Commands* Server::GetCommands() const{
	return m_cmds;
}
void Server::Start(){
	saddrin addr,in_addr;
	Listen(addr);
	while(NewConnection(in_addr)){
		;
	}
	m_cmds = nullptr;
	m_port = 0;
	GetGCSemaphore()->Notify();
	m_garbage_collector->join();
	unsigned len = m_commands.size();
	for(unsigned a=0; a<len; a++){
		m_commands[a]->SetRunning(false);
		m_commands[a]->Notify();	
	}
	delete m_garbage_collector; //created in Listen function
}
bool Server::NewConnection(saddrin in_addr){
	unsigned len = sizeof(saddrin);
	errno = 0;
	int sckt = accept(m_sock,reinterpret_cast<saddr*>(&in_addr),&len);
	TEST_ERR_RET(errno>0,STDMSG("0;36","Stopping"),false);
	TEST_ERR_DO_RET(sckt<0,"Cannot accept socket",close(m_sock),false);
	STDMSG("0;36","Connection: " << inet_ntoa(in_addr.sin_addr) << ":" << ntohs(in_addr.sin_port));
	Commands* cmd = new Commands(); //deleted in GarbageCollector function
	cmd->SetServer(this);
	cmd->SetSocket(sckt);
	thread* thr = new thread(&Commands::Start,cmd); //deleted in Commands destuctor
	cmd->SetThread(thr);
	m_commands.push_back(cmd);
	return true;
}
bool Server::IsRunning() const{
	return m_port;
}
void Server::RemoveCommands(Commands* commands){
	unsigned len = m_commands.size();
	for(unsigned a=0; a<len; a++){
		if(m_commands[a]==commands){
			m_commands.erase(m_commands.begin()+a);
			break;
		}
	}
}
void Server::RemoveGame(Game* game){
	unsigned len = m_games.size();
	for(unsigned a=0; a<len; a++){
		if(m_games[a]==game){
			m_games.erase(m_games.begin()+a);
			delete game; //created in Commands::CreateGame
			break;
		}
	}
}
void Server::Stop(){
	close(m_sock);
}
Semaphore* Server::GetGCSemaphore() const{
	return m_semaphore_gc;
}
char* Server::Receive(int sock) const{
	int rcv = recv(sock,&m_internal_storage,sizeof(char)*MAX_LEN,0);
	if(rcv>=MAX_LEN || rcv<=0 || errno==EAGAIN || errno==EWOULDBLOCK){
		return nullptr;
	}
	m_internal_storage[rcv-1]='\0'; //get rid of \n
	STDMSG("1;33","Received:   " << m_internal_storage);
	char* to_ret = new char[rcv];
	strcpy(to_ret,m_internal_storage);
	return to_ret;
}


void Server::Send(int sock,const char* message) const{
	STDMSG("0;32","Sending:    " << message);
	unsigned length = strlen(message);
	char* extended = new char[length+2];
	strcpy(extended,message);
	extended[length]='\n';
	extended[length+1]='\0';
	int snd = send(sock,extended,sizeof(char)*(length+1),0);
	delete[] extended;
	TEST_ERR(snd<0,"Cannot send message")
}

unsigned Server::GetCountOfGames() const{
	return m_games.size();
}

Game* Server::GetGame(unsigned index) const{
	return m_games[index];
}

void Server::AddGame(Game* game){
	m_games.push_back(game);
}

void Server::AddCommands(Commands* command){
	m_commands.push_back(command);
}
