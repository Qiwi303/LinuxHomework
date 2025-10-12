#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <algorithm>

void initialize(int argc, char** argv){
	if(argc != 2){
		std::cerr<<"invalid input"<<std::endl;
		exit(1);
	}

  	int fd = open(argv[1], O_RDONLY);
	
	if(fd == -1){
		std::cerr<<"failed to open"<<std::endl;
		exit(1);
	}
      	
	int fileInput = dup2(fd, 0);
	if(fileInput == -1){
		std::cerr<<"failed to dup"<<std::endl;
		exit(1);
	}	
}
 
int main(int argc, char** argv){
	
	initialize(argc, argv);

  // read the string 
  	std::string input;
  	std::cin >> input;
  
  // logic to get the reversed string
	std::reverse(input.begin(), input.end()); 			
 
  // print to the console
  	std::cout << input << std::endl;
  	return 0;
}
