#include "semaphore.h"

Semaphore::Semaphore(unsigned count){
	m_count = count;
	m_max_count = count;
}

void Semaphore::Wait(){
	unique_lock<mutex> lock(m_mtx);
	while(!m_count){
		m_cond.wait(lock);
	}
	m_count--;
}

void Semaphore::Notify(){
	unique_lock<mutex> lock(m_mtx);
	m_count++;
	m_cond.notify_one();
}
