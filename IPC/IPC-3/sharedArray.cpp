#include "sharedArray.h"

sharedArray::sharedArray(sharedArray&& other) noexcept:
		name(std::move(other.name)),
		size(other.size),
		fd(other.fd),
		arr(other.arr) {
		
		other.size = 0;
		other.fd = -1;
		other.arr = nullptr;
		other.name.clear();
			
}

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
	if(i < 0 ||  i > size - 1){
		throw std::invalid_argument("enter a correct index");
	}
	return *(arr + i);
}

sharedArray::~sharedArray(){
	if(arr != nullptr){
		munmap(arr, size * sizeof(int));
	}
	
	if(fd != -1){	
		int check = close(fd);
		if(check == -1){
			std::cerr<<"failed to close"<<std::endl;
		}
	}
}

void sharedArray::unlink(){
	int check = shm_unlink(name.c_str());
	if(check == -1 && errno != ENOENT){
		throw std::runtime_error("Failed to unlink");
	}
}

