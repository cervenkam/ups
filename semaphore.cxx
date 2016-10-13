#include "semaphore.h"

Semaphore::Semaphore(unsigned count){
	this->count = count;
	this->max_count = count;
}

void Semaphore::Wait(){
	unique_lock<mutex> lock(mtx);
	while(!count){
		cond.wait(lock);
	}
	count--;
}

void Semaphore::Notify(){
	unique_lock<mutex> lock(mtx);
	if(count<max_count){
		count++;
		cond.notify_one();
	}
}
