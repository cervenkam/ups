#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "commonnetwork.h"
class Client{
	public:
		Client(char* hostname,unsigned port);
		void Connect();
		void Send(char* message);
		char* Receive();
	private:
		void InfinitePrint();
		char* m_hostname;
		unsigned m_port;
		int m_sock;
		char m_internal_storage[MAX_LEN];
};

#endif
