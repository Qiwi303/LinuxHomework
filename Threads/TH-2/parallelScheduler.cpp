#include "parallelScheduler.h"

parallelScheduler::~parallelScheduler(){
	
	pthread_mutex_lock(&mtx);
	stop = true;
	pthread_cond_broadcast(&condVariable);
	pthread_mutex_unlock(&mtx);


	for(int i = 0; i < pool.size(); ++i){
		int check = pthread_join(pool[i], nullptr);
		if(check != 0){
			std::cerr<<"Failed to join pthread"<<std::endl;
		}
	}
}

void parallelScheduler::runTask(void* (*func)(void*), void* arg){
	pthread_mutex_lock(&mtx);
	tasks.push(Task(func, arg));
	
	pthread_cond_signal(&condVariable);
	
	pthread_mutex_unlock(&mtx);

}

void* parallelScheduler::worker(void* arg){
	parallelScheduler* obj = (parallelScheduler*)arg;
	       	while(true){
		pthread_mutex_lock(&obj->mtx);

		while(obj->tasks.empty() && !obj->stop){
			pthread_cond_wait(&obj->condVariable, &obj->mtx);
		}
		if(obj->tasks.empty() && obj->stop){
			pthread_mutex_unlock(&obj->mtx);
		       	return nullptr;
		}

		Task t  = obj->tasks.front();
	      	obj->tasks.pop();	
		
		pthread_mutex_unlock(&obj->mtx);
		t.func(t.arg);

	}
	return nullptr;
}

parallelScheduler::parallelScheduler(const short _cap): cap(_cap){
	if(cap < 1) {
		cap = 10;
		std::cerr<<"Invalid input, current cap set to 10"<<std::endl;
	}
	pool.resize(cap);
	for(int i = 0; i < pool.size(); ++i){
		int check = pthread_create(&pool[i], nullptr, worker, this);
		if(check != 0){
			std::cerr<<"Failed to create thread"<<std::endl;
		}
	}

}
