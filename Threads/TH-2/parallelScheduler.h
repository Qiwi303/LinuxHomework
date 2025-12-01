#pragma once
#include <iostream>
#include <cerrno>
#include <vector>
#include <queue>
#include <pthread.h>
#include <unistd.h>


struct Task{
	void* (*func)(void*);
	void* arg;
	Task(void* (*_func)(void*), void* _arg): func(_func), arg(_arg){} 
};

class parallelScheduler{
public:
	parallelScheduler(const short _cap);
	static void* worker(void* arg);
	void runTask(void* (*func)(void*), void* arg);
	~parallelScheduler();
private:


	short cap;
	std::vector<pthread_t> pool;
	std::queue<Task> tasks;
	pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t condVariable = PTHREAD_COND_INITIALIZER;
	bool stop = false;
	
};
