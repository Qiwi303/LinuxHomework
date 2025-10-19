#include <iostream>
#include <cstring> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

void copy(char** a, char** b, const int argc){
    for (int i = 0; i < argc; ++i) {
        size_t len = std::strlen(a[i]);
        b[i] = new char[len + 1];
        std::strcpy(b[i], a[i]); 
    }
    b[argc] = nullptr;
}

void deleting(char** argv, const int argc){	
    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

void do_command(char** x){
	struct timeval start;
	struct timeval end;
	double before, after;
	
	gettimeofday(&start, NULL);
	before = start.tv_sec + start.tv_usec / 1e6;
	
	pid_t pid = fork();
	if(pid == -1){
		std::cerr<<"Failed to fork"<<std::endl;
		exit(1);
	}

	if(pid == 0){

		execvp(x[1], &x[1]);
		std::cerr<<"execvp was failed"<<std::endl;
		exit(1);
		
	}
	
	if(pid > 0){	
		gettimeofday(&end, NULL);
		after = end.tv_sec + end.tv_usec / 1e6;
		int status;
		
		if(waitpid(pid, &status, 0) == -1){
			std::cerr<<"wait was failed"<<std::endl;
			exit(1);	
		}

		double diff = after - before;

		if(WIFEXITED(status)){
		std::cout<<"Command completed with "<<WEXITSTATUS(status)<<
		       " exit code and took "<<diff<<" seconds"<<std::endl;	
		}else{
			std::cerr<<"execution was failed"<<std::endl;
			exit(1);
		}
	

	}
}	

int main(int argc, char** argv) {
    char** argv1 = new char*[argc + 1]; 
    copy(argv, argv1, argc);

    do_command(argv1);	
	
    deleting(argv1, argc); 
    return 0;
}

