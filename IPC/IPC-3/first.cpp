#include <iostream>
#include "sharedArray.h"
#include <semaphore.h>
#include <atomic>
#include <unistd.h>
#include <signal.h>
#include <sstream>

std::atomic<bool> stop(false);

void sigintHandler(int){
	stop.store(true);
}

int correctCin(){
	int x;
	std::string line;

	while (true) {
    		if(!std::getline(std::cin, line)){
			if(stop.load()){
				throw std::runtime_error("Interrupted by SIGINT");
			}
			throw std::runtime_error("input error");
		}
    		std::stringstream ss(line);
    		if (ss >> x && ss.eof()) {
        		break;
    		}
    		std::cout<<"[first] enter correct integer: ";
	}
	return x;
}

int main(){
	struct sigaction sa{};
	sa.sa_handler = sigintHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	int check = sigaction(SIGINT, &sa, nullptr);
	if(check == -1){
		std::cerr<<"Failed sigaction"<<std::endl;
		exit(1);
	}



	int size = 10;
	std::string name = "test";
	
	try{
		sharedArray arr(size, name);

			
		sem_t* r_sem = sem_open("/sem_test", O_CREAT, 0666, 0);
		if(r_sem == SEM_FAILED){
			std::cerr<<"Failed to create semphore"<<std::endl;
			exit(1);
		}
	
		sem_t* m_sem = sem_open("/mutex_test", O_CREAT, 0666, 1);
		if(m_sem == SEM_FAILED){
			std::cerr<<"Failed to create semphore"<<std::endl;
			exit(1);
		}
		size_t index, value;
		std::string f = "[first] ";
		while(!stop.load()){
		   try{	
			std::cout<<f<<"enter index of "<<name<<": ";
			index = correctCin();
			if(index < 0 ||  index > size - 1){
				std::cerr<<f<<"invalid index"<<std::endl;
				continue;
			}
			std::cout<<f<<name<<"["<<index<<"] = ";
			value = correctCin();
		
			bool locked = false;
			while(true){
				if(sem_wait(m_sem) == 0){
					locked = true;
					break;
				}
				if(errno = EINTR){
					if(stop.load()){
						break;
					}
					continue;
				}
				throw std::runtime_error("sem_wait failed");
			}
			if(!locked) break;

			arr[index] = value;
	        	arr[size] = index;
			sem_post(m_sem);

			sem_post(r_sem);
		     }
	   	   catch(...){ 
			   break;
			}
		}
		
		arr.unlink();
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
