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
		void Wait(unsigned);
	private:
		mutex m_mtx;
		condition_variable m_cond;
		unsigned m_count;
		unsigned m_max_count;
};

#endif
