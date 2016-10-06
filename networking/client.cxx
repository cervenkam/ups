#include "client.h"
#include "commonnetwork.h"
#include <iostream>
#include <cstring>

using namespace std;
Client::Client(char* hostname,unsigned port){
	this->hostname = hostname;
	this->port = port;
}

void Client::Connect(){
	this->sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	TEST_ERR(this->sock<0,"Socket not created")
	hent* host = gethostbyname(this->hostname);
	TEST_ERR(host==NULL,"Host not resolved")
	saddrin addr;
	fill_saddrin(addr,this->port,*reinterpret_cast<inaddr*>(host->h_addr));
	int cnt = connect(this->sock,reinterpret_cast<saddr*>(&addr),sizeof(saddrin));
	TEST_ERR(cnt<0,"Failed to connect")
}

void Client::Send(char* message){
	unsigned length = strlen(message);
	uint32_t netlen = htonl(length);
	int snd = send(this->sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR(snd<0,"Failed to send length of the message")
	snd = send(this->sock,message,sizeof(char)*length,0);
	TEST_ERR(snd<0,"Failed to send message")
}
