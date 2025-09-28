#include<iostream>
#include<unistd.h>
#include<fcntl.h>
int main(int argc, char** argv){

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
	       	perror("open");
		return 1;
	}
	
	const int PAGE_SIZE = 4095;

	std::string buf;
	buf.resize(PAGE_SIZE);
	
	int readSize;

	while((readSize = read(fd, buf.data(), buf.size())) > 0){
		if(write(1, buf.data(), readSize) == -1){
		perror("write");
		close(fd);
		return 1;
		}
	}
	
	close(fd);

	if(readSize < 0){
		perror("read");
		return -1;
	}
		
	return 0;
}

