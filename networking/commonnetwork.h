#ifndef _COMMON_NETWORK_H_
#define _COMMON_NETWORK_H_

#define TEST_ERR_DO(T,P,D) TEST_ERR_DO_RET(T,P,D,)
#define TEST_ERR_DO_RET(T,P,D,R) \
	if(T) { \
		cerr << (P) << endl; \
		D; \
		return R; \
	}
#define TEST_ERR(T,P) TEST_ERR_DO(T,P,)
#define TEST_ERR_RET(T,X,R) TEST_ERR_DO_RET(T,X,,R)

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
typedef struct hostent hent;
typedef struct sockaddr_in saddrin;
typedef struct sockaddr saddr;
typedef struct in_addr inaddr;

void fill_saddrin_any(saddrin &saddr,unsigned port);
void fill_saddrin    (saddrin &saddr,unsigned port,inaddr addr);
#endif
