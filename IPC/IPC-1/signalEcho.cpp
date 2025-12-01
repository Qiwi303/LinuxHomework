#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <pwd.h>
#include <sys/types.h>
#include <cerrno>

void sigHandler(int sig, siginfo_t *info, void* context){
	
	if(sig != SIGUSR1){
		std::cerr << "wrong signal" << std::endl;
		return;
	}

	pid_t senderPid = info->si_pid;
 	uid_t senderUid = info->si_uid;

	struct passwd* pw = getpwuid(senderUid);
        const char* username = pw->pw_name;

	std::cout << "Received a SIGUSR1 signal from process ["
	       	<< senderPid << "] executed by [" << senderUid << "]"
	       	<< "(["<< username << "])." << std::endl; 
	
	ucontext_t* ctx = (ucontext_t*)context;
	greg_t eip = ctx->uc_mcontext.gregs[REG_RIP];
	greg_t eax = ctx->uc_mcontext.gregs[REG_RAX];
	greg_t ebx = ctx->uc_mcontext.gregs[REG_RBX];

	std::cout << "State of the context: EIP = [" << eip
		  << "], EAX = [" << eax 
		  << "], EBX = [" << ebx << "]." << std::endl;
}

int main(){
	pid_t pid = getpid();
	std::cout << "PID: " << pid << std::endl;

	struct sigaction sa = {};
	sa.sa_sigaction = sigHandler;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGUSR1, &sa, nullptr);
	
	while(true) sleep(10); 

	return 0;
}
