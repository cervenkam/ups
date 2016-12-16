#include "networking/server.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <signal.h>
#include <exception>

static Server* server;

void signal_int(int){
	server->Stop();
}

int main(int argc,char** argv){
	try{
		int port = 0;
		if(argc > 1){
			port = atoi(argv[1]);
		}
		if(port>>16 || !port){
			port = 43158;
			std::cout << "Using default port: " << port << std::endl;
		}
		server = new Server(port); //deleted at the end of this function
		signal(SIGINT,signal_int);
		server->Start();
		delete server; //created in the middle of this function
	}catch(std::exception& e) {
		std::cout << "Exception " << e.what() << std::endl;
	}
	return 0;
}
