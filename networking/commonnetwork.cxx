#include "commonnetwork.h"
#include <cstring>

void fill_saddrin_part(saddrin &socket,unsigned port){
	memset(&socket,0,sizeof(saddrin));
	socket.sin_family=AF_INET;
	socket.sin_port=htons(port);
}

void fill_saddrin(saddrin &socket,unsigned port,inaddr iaddr){
	fill_saddrin_part(socket,port);
	socket.sin_addr=iaddr;
}

void fill_saddrin_any(saddrin &socket,unsigned port){
	fill_saddrin_part(socket,port);
	socket.sin_addr.s_addr=htonl(INADDR_ANY);
}
