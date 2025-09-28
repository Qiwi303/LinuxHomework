#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cerrno>

int main(int argc, char** argv){
	if(argc != 3){
		std::cerr<<"simple-copy: missing file operand"<<std::endl;
		return 1;
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
	       	perror("open");
		return 1;
	}

	int fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

	const int PAGE_SIZE = 4095;
	int readSize;

	std::string buf;
	buf.resize(PAGE_SIZE);

	while((readSize = read(fd1, buf.data(), buf.size())) > 0){
		if(write(fd2, buf.data(), readSize) == -1){
		perror("write");
		close(fd1);
		close(fd2);
		return 1;
		}
	}
	
	close(fd1);
	close(fd2);

	if(readSize < 0){
		perror("read");
		return -1;
	}
		
	return 0;
}

