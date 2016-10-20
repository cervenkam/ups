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
void Semaphore::Wait(unsigned timeout){
	unique_lock<mutex> lock(m_mtx);
	if(timeout){
		m_cond.wait_for(lock,milliseconds(timeout));
	}else{
		while(!m_count){
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
