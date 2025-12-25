#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	hostent* host = gethostbyname("httpforever.com");
	if(host == nullptr){
		std::cerr<<"Failed to get host"<<std::endl;
		exit(1);
	}
	
	
	int httpSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if(httpSocket < 0){
		std::cerr<<"socket is failed"<<std::endl;
		exit(1);
	}

	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(80);

	memcpy(&address.sin_addr, host->h_addr, host->h_length);
	
	int connected = connect(httpSocket, (struct sockaddr*)&address, sizeof(address));
	if(connected < 0){
		std::cerr<<"Failed to connect"<<std::endl;
		exit(1);
	}

	const char* message = 
    		"GET / HTTP/1.1\r\n"
    		"Host: httpforever.com\r\n"
    		"Connection: close\r\n"
    		"\r\n";

	ssize_t sended = send(httpSocket, message, strlen(message), 0);
	if(sended < 0){
		std::cerr<<"Failed to send"<<std::endl;
		exit(1);
	}
	
	int fd = open("httpforever.html", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	ssize_t written;
	ssize_t received = 1;
	char buff[4096];

	while(received > 0){
		received = recv(httpSocket, buff, sizeof(buff), 0);
		if(received < 0){
			std::cerr<<"Failed to receive"<<std::endl;
			exit(1);
		}

		written = write(fd, buff, received);
		if(written < 0){
			std::cerr<<"Failed to write"<<std::endl;
			exit(1);
		}
	}

	ssize_t closed = close(fd);
	if(closed < 0){
		std::cerr<<"Failed to close"<<std::endl;
		exit(1);
	}
	
	closed = close(httpSocket);
	if(closed < 0){
		std::cerr<<"Failed to close"<<std::endl;	
		exit(1);
	}

	return 0;
		
}
