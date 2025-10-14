#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <vector>

std::vector<int> initialize(int argc, char** argv){
	if(argc != 2){
		std::cerr<<"invalid input"<<std::endl;
		exit(1);
	}

  	int fd1 = open(argv[1], O_WRONLY | O_TRUNC);
	
	if(fd1 == -1){
		std::cerr<<"failed to open"<<std::endl;
		exit(1);
	}
      	
	int fd2 = dup(fd1);
	if(fd2 == -1){
		std::cerr<<"failed to dup"<<std::endl;
		exit(1);
	}
	
	
	std::vector<int> fd = {fd1, fd2}; 

	return fd;	
}
 
int main(int argc, char** argv){
	
	std::vector<int> fd = initialize(argc, argv);
  
	const char* line1 = "first line\n";
	const char* line2 = "second line\n";

	int written = write(fd[1], line1, 11);
	if(written == -1){
		std::cerr<<"failed to open"<<std::endl;
		exit(1);
	}

	written = write(fd[0], line2, 12);
	if(written == -1){
		std::cerr<<"failed to open"<<std::endl;
		exit(1);
	}
	
	int closed0 = close(fd[0]);
	if(closed0 == -1){
		std::cerr<<"failed to close"<<std::endl;
		exit(1);
	}
	
	int closed1 = close(fd[1]);
	if(closed1 == -1){
		std::cerr<<"failed to close"<<std::endl;
		exit(1);
	}

  	return 0;
}
