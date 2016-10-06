#ifndef _CLIENT_H_
#define _CLIENT_H_

class Client{
	public:
		Client(char* hostname,unsigned port);
		void Connect();
		void Send(char* message);
	private:
		char* hostname;
		unsigned port;
		int sock;
};

#endif
