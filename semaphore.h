#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

using namespace std;
class Semaphore{
	public:
		Semaphore(unsigned count=1);
		void Notify();
		void Wait();
	private:
		mutex mtx;
		condition_variable cond;
		unsigned count;
		unsigned max_count;
};

#endif
