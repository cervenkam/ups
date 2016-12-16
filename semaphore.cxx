#include "semaphore.h"
#include <iostream>

Semaphore::Semaphore(unsigned count){
	m_count = count;
	m_max_count = count;
}

void Semaphore::Wait(){
	Wait(0);
}
bool Semaphore::Wait(unsigned timeout){
	bool ret_value;
	std::unique_lock<std::mutex> lock(m_mtx);
	if(timeout){
		ret_value = m_cond.wait_for(lock,std::chrono::milliseconds(timeout))==std::cv_status::timeout;
	}else{
		while(!m_count){
			m_cond.wait(lock);
		}
		ret_value = false;
	}
	m_count--;
	return ret_value;
}

void Semaphore::Notify(){
	std::unique_lock<std::mutex> lock(m_mtx);
	m_count++;
	m_cond.notify_one();
}
