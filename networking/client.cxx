#include "client.h"
#include "commonnetwork.h"
#include <iostream>
#include <cstring>
#include <thread>

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
	thread* thr = new thread(&Client::InfinitePrint,this);
	//TODO housekeeping
}

void Client::Send(char* message){
	unsigned length = strlen(message);
	uint32_t netlen = htonl(length);
	int snd = send(this->sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR(snd<0,"Failed to send length of the message")
	snd = send(this->sock,message,sizeof(char)*length,0);
	TEST_ERR(snd<0,"Failed to send message")
}

void Client::InfinitePrint(){
	while(internal_storage[0]){
		cout << Receive() << endl;
	}
}

char* Client::Receive(){
	uint32_t netlen;
	int rcv = recv(sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR_RET(rcv<0,"Cannot get message length",NULL)
	unsigned length = ntohl(netlen);
	TEST_ERR_RET(length>=MAX_LEN,"Message too long",NULL)
	rcv = recv(sock,&internal_storage,sizeof(char)*MAX_LEN,0);
	TEST_ERR_RET(rcv<0,"Cannot get message",NULL)
	internal_storage[length]='\0';
	return internal_storage;
}
