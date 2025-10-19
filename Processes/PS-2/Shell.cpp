#include "Shell.h"

void Shell::checkSysError(const int num, const std::string& error){
	if(num < 0){
		throw SysCallError("Failed to " + error);
	}
}	

void Shell::runShell(){
	while(true){
		readAndSplit();
		try{
		    splitAndCallCommands();
		}
		catch(const SysCallError& e){
			std::cerr<<e.what()<<std::endl;
		}	
		if(finished) return;
	}
}
	
void Shell::readAndSplit(){	
	argv.clear();
	tokens.clear();
	
	std::string command;
	std::getline(std::cin, command);
	std::string tmp;

	for (int i = 0; i < command.size(); ++i) {
		if (command[i] == ';'){
			if(!tmp.empty()){
				tokens.push_back(tmp);
				tmp.clear();
			}
			tokens.push_back(";");	
		}	

		else if (command[i] == '>' && ( i == (command.size() - 1) || command[i+1] != '>')){
			if(!tmp.empty()){
				tokens.push_back(tmp);
				tmp.clear();
			}
			tokens.push_back(">");

		}	

		else if(command[i] == '&' || command[i] == '|' || command[i] == '>'){
			if(command[i] == command[i+1]){
					if(!tmp.empty()){
						tokens.push_back(tmp);
					}
					std::string opTmp;
					opTmp += std::string(1, command[i]) + command[i+1];
					tokens.push_back(opTmp);
					++i;
			}		
		}

		else if (command[i] == ' ') {
        		if (!tmp.empty()) {
            			tokens.push_back(tmp);
            			tmp.clear();
        		}
    		} else {
        		tmp += command[i];
    		}
	}
	
	if (!tmp.empty()) {
    		tokens.push_back(tmp);
	}

	for(auto& x: tokens) argv.push_back(const_cast<char*>(x.c_str()));
	argv.push_back(nullptr);
}	

void Shell::redirectOut(const char path[], bool append){
	int fd;
	if(append){	
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
		checkSysError(fd, "open");
	}

	else{
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		checkSysError(fd, "open");
	}
	
	checkSysError(fd, "open");
	checkSysError(dup2(fd, STDOUT_FILENO), "dup2.1");
	checkSysError(dup2(fd, STDERR_FILENO), "dup2.2");
	checkSysError(close(fd), "close");

}	

void Shell::execCommand(std::vector<char*>& argv){
	bool silentMod = false;
		
	if(argv.empty() || argv[0] == nullptr) return;

	if(strcmp(argv[0], "exit") == 0){
	       	finished = true;
		return;
	}


	if(strcmp(argv[0], "silent") == 0){
		argv.erase(argv.begin());
		silentMod = true;
	}
	
	pid_t pid = fork();
	checkSysError(pid, "pid");
	

	if(pid == 0){

		if(argv.size() >= 3){
			bool checkOp1 = strcmp(argv[argv.size() - 3], ">") == 0;
			bool checkOp2 = strcmp(argv[argv.size() - 3], ">>") == 0;
				if(checkOp1){
					redirectOut(argv[argv.size() - 2], false);
					silentMod = false;
					argv.erase(argv.end() - 2);
					argv.erase(argv.end() - 2);
				}	
				else if(checkOp2){
					redirectOut(argv[argv.size() - 2], true);
					silentMod = false;
					argv.erase(argv.end() - 2);
					argv.erase(argv.end() - 2);
			}
		}
		
		int fd;		
		if(silentMod){
			int childPid = getpid();
			std::string fileName = std::to_string(childPid) + ".log";
			redirectOut(fileName.c_str(), false);
		}


		const char* oldPath = getenv("PATH");
		char cwd[PATH_MAX];

		if (getcwd(cwd, sizeof(cwd)) == NULL){
			std::cerr<<"Failed to getcwd"<<std::endl;
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
		checkSysError(waitpid(pid, &status, 0), "wait");
		
		if(WIFEXITED(status)){
			lastExitCode = WEXITSTATUS(status);
		}
		else{
			lastExitCode = 1;
			std::cerr<<"execution was failed"<<std::endl;
		}
	}

}		

void Shell::splitAndCallCommands(){
	std::vector<char*> tmp;
	
	for(int i = 0; i < argv.size() - 1; i++){	
		
		if(strcmp(argv[i], "&&") == 0){
			if(!tmp.empty()){	
				tmp.push_back(nullptr);
				execCommand(tmp);
				tmp.clear();
				if(lastExitCode == 1){
					return;
				}
			}
		}	
		
		else if(strcmp(argv[i], "||") == 0){
			if(!tmp.empty()){
				tmp.push_back(nullptr);
				execCommand(tmp);
				tmp.clear();				
				if(lastExitCode == 0){
					return;
				}
			}
		}	


		else if(strcmp(argv[i], ";")  == 0){
			if(!tmp.empty()){
				tmp.push_back(nullptr);
				execCommand(tmp);
				tmp.clear();
			}
		} else {
			tmp.push_back(argv[i]);
		}	
	}

	if(!tmp.empty()){
		tmp.push_back(nullptr);
		execCommand(tmp);
	}

}
