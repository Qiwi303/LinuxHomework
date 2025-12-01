#include <iostream>
#include "parallelScheduler.h"

void* printString(void* arg){
	std::string* str = (std::string*)(arg);
	std::cout<<*str<<std::endl;
	return nullptr;
}

void* printSleepString(void* arg){
	sleep(5);
	std::string* str = (std::string*)(arg);
	std::cout<<*str<<std::endl;
	return nullptr;
}


int main(){
	std::string str1 = "1 I like Rau";
	std::string str2 = "2 I like Rau";
	std::string str3 = "3 I like Rau";
	std::string str4 = "4 I like Rau";
	
	parallelScheduler ps(2);
	ps.runTask(printString, &str1);
	ps.runTask(printSleepString, &str2);
	ps.runTask(printString, &str3);
	ps.runTask(printSleepString, &str4);

	return 0;
}
