#ifdef SERVER
	#include "networking/server.h"
#else
	#include "networking/client.h"
#endif
#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;
int main(int argc,char** argv){
	int port = 0;
	if(argc > 1){
		port = atoi(argv[1]);
	}
#ifdef SERVER
	Server* server = new Server(port);
	server->Start();
	delete server;
#else
	Client client(argc>2?argv[2]:nullptr,port);
	client.Connect();
	char line[512];
	while(fgets(line,512,stdin)){
		client.Send(line);
	}
#endif
	return 0;
}
