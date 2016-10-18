#include "client.h"
#include "commonnetwork.h"
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;
Client::Client(char* hostname,unsigned port){
	m_hostname = hostname;
	m_port = port;
	m_internal_storage[0]=' ';
}

void Client::Connect(){
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	TEST_ERR(m_sock<0,"Socket not created")
	hent* host = gethostbyname(m_hostname==nullptr?"localhost":m_hostname);
	TEST_ERR(host==nullptr,"Host not resolved")
	saddrin addr;
	fill_saddrin(addr,m_port,*reinterpret_cast<inaddr*>(host->h_addr));
	int cnt = connect(m_sock,reinterpret_cast<saddr*>(&addr),sizeof(saddrin));
	TEST_ERR(cnt<0,"Failed to connect")
	thread thr(&Client::InfinitePrint,this);
	thr.detach();
}

void Client::Send(char* message){
	unsigned length = strlen(message);
	uint32_t netlen = htonl(length);
	int snd = send(m_sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR(snd<0,"Failed to send length of the message")
	snd = send(m_sock,message,sizeof(char)*length,0);
	TEST_ERR(snd<0,"Failed to send message")
}

void Client::InfinitePrint(){
	do{
		cout << Receive() << endl;
	}while(m_internal_storage[0]);
}

char* Client::Receive(){
	uint32_t netlen;
	int rcv = recv(m_sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR_RET(rcv<0,"Cannot get message length",nullptr)
	unsigned length = ntohl(netlen);
	TEST_ERR_RET(length>=MAX_LEN,"Message too long",nullptr)
	rcv = recv(m_sock,&m_internal_storage,sizeof(char)*MAX_LEN,0);
	TEST_ERR_RET(rcv<0,"Cannot get message",nullptr)
	m_internal_storage[length]='\0';
	return m_internal_storage;
}
