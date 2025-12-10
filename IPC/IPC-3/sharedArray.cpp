#include "sharedArray.h"


sharedArray::sharedArray(const size_t _size, const std::string name_): size(_size), name(name_){
	//резервируем последнее места для передачи индекса в другой процесс
	size++;
	if(size < 1 || size > 1000000000){
		throw std::invalid_argument("size must be between 1 and 1 000 000 000");
	}
	
	const char* _name = name.c_str();
	bool creator = false;

	fd = shm_open(_name, O_CREAT | O_EXCL | O_RDWR, 0666);
	
	if(fd >= 0){
		int check = ftruncate(fd, size*sizeof(int));
		
		if(check == -1){
			close(fd);
			shm_unlink(_name);
			throw std::runtime_error("Failed to truncate");
		}

		creator = true;
		
	}
	
	else if(errno == EEXIST){
		
		fd = shm_open(_name, O_RDWR, 0666);
		if(fd < 0){
			throw std::runtime_error("Failde to open");
		}

					
		struct stat sb;
		int check = fstat(fd, &sb);
		if(check == -1){
			close(fd);	
			throw std::runtime_error("Failed to get stat");
		}	

		if(sb.st_size != size*sizeof(int)){
			close(fd);		
			throw std::runtime_error("invalid size of existing array");
		}

	}

	else{
		throw std::runtime_error("Failed to open");
	}

	
	void* ptr = mmap(nullptr, size*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED){
		close(fd);
		if(creator) shm_unlink(_name);
		throw std::runtime_error("Failed to map");
	}	
	arr = (int*)ptr;
}

int& sharedArray::operator[](int i){
	return *(arr + i);
}

sharedArray::~sharedArray(){
	munmap(arr, size * sizeof(int));
	int check = close(fd);

	if(check == -1){
		std::cerr<<"failed to close"<<std::endl;
	}
}
