#define __FIRST_TEST__(x,y) void run##x(){\
	const char* mess = y;
#define TEST(z,x,y) }__FIRST_TEST__(x,y) \
	run##z();
#define FIRST_TEST(x) __FIRST_TEST__(0,x)
#define NAMESPACE(x) namespace __ns##x{
#define START(x,y,z) void main(){\
		__ns##x::main(); \
		run##z(); \
	}
#define FIRST_START(x) void main() { \
		run##x(); \
	}
#define EXEC(x) int main() { \
		__ns##x::main(); \
		return 0; \
	}
#define DEBUG(x)//cout << x;
#include <iostream>
#ifdef __unix__
	#define __ASSERT__(x,q,w,y) if((x) q (y)){ \
			cerr << "\033[0;31m[FAIL] " << mess << " [" << __FILE__ << ":" << __LINE__ << "] => " << (x) << " " << #w << " " << (y) << "\033[0m" << endl; \
		}else{ \
			cout << "\033[0;32m[ OK ] " << mess << " [" << __FILE__ << ":" << __LINE__ << "] => " << (x) << " " << #w << " " << (y) << "\033[0m" << endl; \
		}
#else
	#define __ASSERT__(x,q,w,y) if((x) q (y)){ \
			cerr << "[FAIL] " << mess << " [" << __FILE__ << ":" << __LINE__ << "] => " << (x) << " " << #w << " " << (y) << endl; \
		}else{ \
			cout << "[ OK ] " << mess << " [" << __FILE__ << ":" << __LINE__ << "] => " << (x) << " " << #w << " " << (y) << endl; \
		}
#endif
#define ASSERT(x,y) __ASSERT__(x,!=,==,y)
#define ASSERT_NOT(x,y) __ASSERT__(x,==,!=,y)
#define END }
using namespace std;
