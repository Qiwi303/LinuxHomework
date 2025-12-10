#include <iostream>
#include "sharedArray.h"
#include <semaphore.h>

int main(){
	int size = 10;
	std::string name = "test";
	
	try{
		sharedArray arr(size, name);
	
		sem_t* r_sem = sem_open("/sem_test", 0);
		if(r_sem == SEM_FAILED){
			std::cerr<<"Failed to create semphore"<<std::endl;
			exit(1);
		}
	
		sem_t* m_sem = sem_open("mutex_test", 1);
		if(m_sem == SEM_FAILED){
			std::cerr<<"Failed to create semphore"<<std::endl;
			exit(1);
		}
		std::string s = "[second] ";
		while(true){
			sem_wait(r_sem);
			sem_wait(m_sem);
	
			std::cout<<s<<name<<"["<<arr[size]<<"] = "<< arr[arr[size]]<<std::endl;
		
			sem_post(m_sem);
		}	
	
	}
	catch(const std::runtime_error& e){
		std::cerr<<"runtime error: "<<e.what()<<std::endl;
	}
	catch(const std::invalid_argument& e){
		std::cerr<<"invalid argument: "<<e.what()<<std::endl;
	}
	catch(...){
		std::cerr <<"Unknown error"<<std::endl;
	}
	
	return 0;
}
