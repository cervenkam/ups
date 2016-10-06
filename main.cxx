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
#ifdef SERVER
	Server* server = new Server(34567);
	server->Start();
	delete server;
#else
	Client client(argv[1],34567);
	client.Connect();
	char line[512];
	while(fgets(line,512,stdin)){
		client.Send(line);
	}
#endif
	return 0;
}
