#pragma once
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <cerrno>

class sharedArray{
public:
	sharedArray(const size_t _size, const std::string name);
	int& operator[](int i);
	~sharedArray();
	

	sharedArray(const sharedArray&) = delete;
	sharedArray& operator=(const sharedArray&) = delete;
private:
	size_t size; 
	int fd;
	int* arr;
	std::string name;	
};
