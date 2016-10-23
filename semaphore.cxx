#include "semaphore.h"
#include <iostream>

using namespace std;
using std::chrono::milliseconds;
Semaphore::Semaphore(unsigned count){
	m_count = count;
	m_max_count = count;
}

void Semaphore::Wait(){
	Wait(0);
}
bool Semaphore::Wait(unsigned timeout){
	bool ret_value;
	unique_lock<mutex> lock(m_mtx);
	if(timeout){
		ret_value = m_cond.wait_for(lock,milliseconds(timeout))==cv_status::timeout;
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
	unique_lock<mutex> lock(m_mtx);
	m_count++;
	m_cond.notify_one();
}
