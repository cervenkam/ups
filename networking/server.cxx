#include "server.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "commonnetwork.h"

using namespace std;
Server::Server(unsigned port){
	this->port = port;
}

void Server::Start(){
	int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
	TEST_ERR(sock<0,"Socket not created")
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	saddrin addr, in_addr;
	fill_saddrin_any(addr,this->port);
	int bnd = bind(sock,reinterpret_cast<saddr*>(&addr),sizeof(saddrin));
	TEST_ERR(bnd<0,"Socket not binded")
	int lsn = listen(sock, 10);
	TEST_ERR(lsn<0,"Cannot listen")
	while(1){
		unsigned len = sizeof(saddrin);
		errno = 0;
		int sckt = accept(sock,reinterpret_cast<saddr*>(&in_addr),&len);
		TEST_ERR(errno>0,strerror(errno));
		TEST_ERR_DO(sckt<0,"Cannot accept socket",close(sock))
		cout << "Connection from " << inet_ntoa(in_addr.sin_addr) << ":" << ntohs(in_addr.sin_port) << endl;
		if(fork() == 0){
			while(1){
				cout << Receive(sckt);
			}
		}
		//close(sckt);
	}
}
char* Server::Receive(int sock){
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
void Server::Send(int sock,char* message){
	unsigned length = strlen(message);
	uint32_t netlen = htonl(length);
	int snd = send(sock,&netlen,sizeof(uint32_t),0);
	TEST_ERR(snd<0,"Cannot send message length")
	snd = send(sock,message,sizeof(char)*length,0);
	TEST_ERR(snd<0,"Cannot send message")
}
