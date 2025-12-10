#include <iostream>
#include "sharedArray.h"
#include <semaphore.h>

int main(){
	int size = 10;
	std::string name = "test";
	
	try{
		sharedArray arr(size, name);

			
		sem_t* r_sem = sem_open("/sem_test", O_CREAT, 0666, 0);
		if(r_sem == SEM_FAILED){
			std::cerr<<"Failed to create semphore"<<std::endl;
			exit(1);
		}
	
		sem_t* m_sem = sem_open("mutex_test", O_CREAT, 0666, 1);
		if(m_sem == SEM_FAILED){
			std::cerr<<"Failed to create semphore"<<std::endl;
			exit(1);
		}
		size_t index, value;
		std::string f = "[first] ";
		while(true){
			std::cout<<f<<"enter index of "<<name<<": ";
			std::cin>>index;
			if(index < 0 ||  index > size - 1){
				std::cerr<<"invalid index"<<std::endl;
				continue;
			}
			std::cout<<f<<name<<"["<<index<<"] = ";
			std::cin>>value;
		
			sem_wait(m_sem);
			arr[index] = value;
	        	arr[size] = index;
			sem_post(m_sem);

			sem_post(r_sem);
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
