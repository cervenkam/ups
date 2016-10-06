#ifndef _SERVER_H_
#define _SERVER_H_

#define MAX_LEN 512
class Server{
	public:
		void Start();
		char* Receive(int sock);
		Server(unsigned port);
		void Send(int sock,char* message);
	private:
		unsigned port;
		char internal_storage[MAX_LEN];
};

#endif
