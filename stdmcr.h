#ifndef _STDMCR_H_
#define _STDMCR_H_

#include <ctime>
//OS dependency solver - can be used for GUI too
#define OUT(x) cout << x

#ifdef __unix__
	#define COLOR(x,y) OUT("\033[" << x << "m" << y << "\033[0m")
	#define MOVE(x,y) OUT("\033[" << x << "f" << y)
	#define __MOVE_D__(x,y) "\033[" << #x << y
	#define CLEAR OUT("\033[2J")
	#define RAW_READ system("/bin/stty raw");
	#define COOKED_READ system("/bin/stty cooked");
#else
	//Windows doesnt know about bash escape sequences
	#define COLOR(x,y) OUT(y)
	#define MOVE(x,y) OUT(y)
	#define __MOVE_D__(x,y)
	#define CLEAR
	#define RAW_READ
	#define COOKED_READ
#endif
#define MOVEA(x) __MOVE_D__(x,"A")
#define MOVEB(x) __MOVE_D__(x,"B")
#define MOVEC(x) __MOVE_D__(x,"C")
#define MOVED(x) __MOVE_D__(x,"D")
#define STDMSG(X,Y) COLOR(X,"[ " << (long int)(time(0)) << " ] " << Y << endl)

#endif
