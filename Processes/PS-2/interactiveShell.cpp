#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <climits>

std::vector<char*> toChar(std::vector<std::string>& tokens){	
	std::vector<char*> argv;
	for(auto& x: tokens) argv.push_back(const_cast<char*>(x.c_str()));
	argv.push_back(nullptr);
	
	return argv;
}

std::vector<std::string> readAndSplit(std::string& command){
	std::getline(std::cin, command);
	std::vector<std::string> tokens;
	std::string tmp;

	for (char c : command) {
		if (c == ';'){
			if(!tmp.empty()){
				tokens.push_back(tmp);
				tmp.clear();
			}
			tokens.push_back(";");	
		}
		else if (c == ' ') {
        		if (!tmp.empty()) {
            			tokens.push_back(tmp);
            			tmp.clear();
        		}
    		} else {
        		tmp += c;
    		}
	}
	
	if (!tmp.empty()) {
    	tokens.push_back(tmp);
	}

	return tokens;	
}	

int execCommand(std::vector<char*>& argv){
	bool silentMod = false;
		
	if(argv.empty() || argv[0] == nullptr) return 1;

	if(strcmp(argv[0], "exit") == 0) return 0;
 	
	else if(strcmp(argv[0], "silent") == 0){
		argv.erase(argv.begin());
		silentMod = true;
	}
	pid_t pid = fork();
	
	if(pid == -1){
		std::cerr<<"Failed to fork"<<std::endl;
	}

	if(pid == 0){
		int fd;		
		if(silentMod){
			int childPid = getpid();
			std::string fileName = std::to_string(childPid) + ".log";
			fd = open(fileName.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if(fd == -1){
				std::cerr<<"failed to open"<<std::endl;
				exit(1);
			}
			int checkDup = dup2(fd, STDOUT_FILENO);
			if(checkDup == -1){
				std::cerr<<"failed to dup"<<std::endl;
				exit(1);
			}	
			checkDup = dup2(fd, STDERR_FILENO);
		        
			if(checkDup == -1){
		       		std::cerr<<"failed to dup"<<std::endl;
		 		exit(1);
			}

	       		int closed = close(fd);
			if(closed == -1){
				std::cerr<<"failed to close"<<std::endl;
				exit(1);
			}
			silentMod = false;	
		}

		const char* oldPath = getenv("PATH");
		char cwd[PATH_MAX];

		if( getcwd(cwd, sizeof(cwd)) == NULL){
			std::cerr<<"failed getcwd"<<std::endl;
			exit(1);
		}

		std::string newPath= std::string(cwd);
		if(oldPath){
			newPath += ":" + std::string(oldPath);
		}

		int check2 = setenv("PATH", newPath.c_str(), 1);
		if(check2 !=0){
			std::cerr<<"setenv was failed"<<std::endl;
			exit(1);
		}	

		int check = execvp(argv[0], &argv[0]);
		if(check == -1){
			std::cerr<<"Failed to execute"<<std::endl;
			exit(1);
		}
	}

	if(pid > 0){	
		int status;
		int check = waitpid(pid, &status, 0);
		if(check == -1){
			std::cerr<<"failed to wait"<<std::endl;
			exit(1);
		}

		if(!WIFEXITED(status)){
			std::cerr<<"execution was failed"<<std::endl;
		}
	}

		return 1;	
}		

int splitAndCallCommands(std::vector<std::string>& tokens){
	std::vector<std::string> tmp;
	for(int i = 0; i < tokens.size(); i++){
		if(tokens[i] == ";"){
			if(!tmp.empty()){
				std::vector<char*> argv = toChar(tmp);
				int check = execCommand(argv);
				if(check == 0) return 0;
				tmp.clear();
			}
		} else {
			tmp.push_back(tokens[i]);
		}	
	}

	if(!tmp.empty()){
		std::vector<char*> argv = toChar(tmp);
		return execCommand(argv);
	}	
	return 1;	
}	

int main(){
	std::string commands;
	while(true){
		std::vector<std::string> tokens = readAndSplit(commands);
		int check = splitAndCallCommands(tokens);
		if(check == 0) break;
	}

	return 0;
}
