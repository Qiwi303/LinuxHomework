#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/wait.h>

bool checkIfPrime(const int num){
	if(num == 2){
		return true;
	}
	for(int i = 2; i*i <= num; ++i){
		if(num%i == 0) return false;
	}

	return true;
}

int findPrime(const int num){
	int count = 1;
	int i = 2;
	while(count < num){
		i++;
		if(checkIfPrime(i)) count++;
	}

	return i;
}

int main(){
	int parentToChild[2];
	int childToParent[2];
	int num;
	std::string m;
	std::string p = "[Parent]";
	std::string c = "[Child]";

	int check = pipe(parentToChild);
	if(check < 0){
		std::cerr<<"Failed to pipe \n";
	}
		
	check = pipe(childToParent);
	if(check < 0){
		std::cerr<<"Failed to pipe \n";
	}
	
	pid_t pid = fork();

	if(pid < 0){
		std::cerr<<"Failed to fork \n";
		exit(1);
	}

	if(pid > 0){
		check = close(parentToChild[0]);
		if(check < 0){
			std::cerr<<"Failed to close \n";
			exit(1); 
		}
		check = close(childToParent[1]);
		if(check < 0){
			std::cerr<<"Failed to close \n";
			exit(1);
		}

		while(true){

			std::cout<<p<<" Please enter the number: ";
			std::cin>>m;
			
				
			size_t pos;
				
			if(m == "exit"){
				num = -1;
				check = write(parentToChild[1], &num, sizeof(num));
				if(check < 0){
					std::cerr<<"Failed to write \n";
					exit(1);
				}
				wait(nullptr);
				exit(0);
			}

			try{	
				num = std::stoi(m, &pos);
				if(num < 1 || pos != m.size()){
					throw std::invalid_argument("invalid input");
				}
			}	
			catch(...){
				std::cerr<<"invalid input \n";
				continue;
			}

			std::cout<<p<<" Sending "<<num<<" to the child process... \n";

			check = write(parentToChild[1], &num, sizeof(num));
			if(check < 0){
				std::cerr<<"Failed to write \n";
				exit(1);
			}

			std::cout<<p<<" Waiting for the response from the child process... \n";	
			
			int res;
			check = read(childToParent[0], &res, sizeof(res));
			if(check < 0){
				std::cerr<<"Failed to read \n";
				exit(1);
			}

			std::cout<<p<<" Received calculation result of prime("<<num<<") = "<<res<<"... \n";
				
		}
	}

	if(pid == 0){	
		check = close(parentToChild[1]);
		if(check < 0){
			std::cerr<<"Failed to close \n";
			exit(1);
		}
		check = close(childToParent[0]);
		if(check < 0){
			std::cerr<<"Failed to close \n";
			exit(1);
		}

		while(true){
			check = read(parentToChild[0], &num, sizeof(num));
			if(check < 0){
				std::cerr<<"Failed to read \n";
				exit(1);
			}
		
			if(num == -1){
				exit(0);
			}

			std::cout<<c<<" Calculating "<<num<<"-th prime number ... \n";	
			int res = findPrime(num);
			
			check = write(childToParent[1], &res, sizeof(res));
			std::cout<<c<<" Sending calculation result of prime("<<num<<") ... \n";

			if(check < 0){
				std::cerr<<"Failed to write \n";
				exit(1);
			}
		}					
	}	
	
	return 0;
}
