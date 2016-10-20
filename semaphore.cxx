#include "semaphore.h"

using std::chrono::milliseconds;
Semaphore::Semaphore(unsigned count){
	m_count = count;
	m_max_count = count;
}

void Semaphore::Wait(){
	Wait(0);
}
void Semaphore::Wait(int timeout){
	unique_lock<mutex> lock(m_mtx);
	while(!m_count){
		if(timeout){
			m_cond.wait_for(lock,milliseconds(timeout));
		}else{
			m_cond.wait(lock);
		}
	}
	m_count--;
}

void Semaphore::Notify(){
	unique_lock<mutex> lock(m_mtx);
	m_count++;
	m_cond.notify_one();
}
