#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

class Semaphore{
	public:
		Semaphore(unsigned count=1);
		void Notify();
		void Wait();
		bool Wait(unsigned);
	private:
		std::mutex m_mtx;
		std::condition_variable m_cond;
		unsigned m_count;
		unsigned m_max_count;
};

#endif
