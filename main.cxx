#include "networking/server.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <signal.h>
using namespace std;

static Server* server;

void signal_int(int){
	server->Stop();
}

int main(int argc,char** argv){
	int port = 0;
	if(argc > 1){
		port = atoi(argv[1]);
	}
	server = new Server(port); //deleted at the end of this function
	signal(SIGINT,signal_int);
	server->Start();
	delete server; //created in the middle of this function
	return 0;
}
