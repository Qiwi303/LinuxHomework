#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<vector>
#include<queue>
#include<algorithm>
struct comporator {
	bool operator()(std::pair<int, std::vector<char>> i, std::pair<int, std::vector<char>> j) {
		return i.second > j.second;
	}
};


void quickSort(std::vector<std::vector<char>> &lines){
	std::sort(lines.begin(), lines.end());
}

std::vector<char> readLine(int &fd) {
	std::vector<char> buf;

	while(true) {
		char x;
		int readBytes = read(fd, &x, 1);

		if(readBytes == -1) {
			perror("read");
			exit(-1);
		}
		if(readBytes == 0) {	 
			break;
		}
		
		buf.push_back(x);
		
		if(int(x) == int('\n')) {
			break;	
		}
	}
	return buf;

}


void mergeChunks(const int start, const int end, int& current_chunk) {
	std::priority_queue<std::pair<int, std::vector<char>>, std::vector<std::pair<int,std::vector<char>>>, comporator> minHeap;
	std::vector<std::pair<int, int>> fds;
	char filename[50];
	for(int i = start; i <= end; ++i) { 
		sprintf(filename, "chunk_%08d.tmp", i);
		
		int fd = open(filename, O_RDONLY, 0600);
		fds.push_back(std::pair<int, int>(fd, i));
	}
	sprintf(filename, "chunk_%08d.tmp", current_chunk);

	int fdMerged = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
	
	std::vector<char> buf;

	for(int i = 0 ; i < end - start; i++) {
		buf = readLine(fds[i].first);
		minHeap.push(std::pair<int, std::vector<char>>(i, buf));
	}
	
	int readFrom;
		
	while(!minHeap.empty()) {
		readFrom = minHeap.top().first;
		
		std::cout<<"readFrom - "<<readFrom<<std::endl;

		write(fdMerged, minHeap.top().second.data(), minHeap.top().second.size());

		minHeap.pop();
		buf = readLine(fds[readFrom].first);
		if(!buf.empty()){
		minHeap.push(std::pair<int, std::vector<char>>(readFrom, buf));
		}
	}	



	while(!fds.empty()){
		close(fds.back().first);
		sprintf(filename, "chunk_%08d.tmp", fds.back().second);
		int unCheck = unlink(filename);

		if(unCheck < 0){
			perror("unlink");
			exit(1);	
		}
		
			
		fds.pop_back();
	}

	close(fdMerged);
	current_chunk++;	
		
}

int main(int argc, char** argv){

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
	       	perror("open");
		return 1;
	}
	
	const int BUFFER_SIZE = 8192;
	int readSize; 

	std::string buf;
	buf.resize(BUFFER_SIZE);

	int chunk_counter = 1;

	while(true){	
	readSize = read (fd, buf.data(), BUFFER_SIZE);
	if(readSize == -1){
		perror("read");
		return 1;
	}
	
	if(readSize == 0) {
		break;
	}

	int last_newline_pos = -1;
        for(int i = readSize - 1; i >= 0; i--){
            if(buf[i] == '\n'){
                last_newline_pos = i;
                break;
            }
        }
        
        if(last_newline_pos == -1){
            std::cerr << "Warning: No newline found in chunk, skipping..." << std::endl;
            last_newline_pos = readSize - 1;
        }
        
        int bytes_to_rewind = readSize - last_newline_pos - 1;
        if(bytes_to_rewind > 0){
            if(lseek(fd, -bytes_to_rewind, SEEK_CUR) == -1){
                perror("lseek");
                close(fd);
                return 1;
            }
        }

	std::vector<std::vector<char>> lines;
	std::vector<char> line;
	for(int j = 0; j <= last_newline_pos; j++){
		line.push_back(buf[j]);
		if( int(buf[j]) == int('\n')){
			lines.push_back(line);
			line.clear();		
		}
	}

	if(!line.empty()) {
		line.push_back('\n');
		lines.push_back(line);
	}

	if(lines.empty()){
		std::cerr << "Warning: sjfhaskjf" << std::endl;
		continue;
	}

	quickSort(lines);
	
	
	char filename[50];

	sprintf(filename, "chunk_%08d.tmp", chunk_counter);
	
	int fdTMP = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);

	if(fdTMP == -1){
		perror("open");
		return 1;
	}
	
	for(auto l: lines) {
		write(fdTMP, l.data(), l.size());
	}	

	close(fdTMP);
	chunk_counter++;
	
	}

	close(fd);

	for(int k = 1; k < chunk_counter; k+=120){
		if(chunk_counter - k < 120){
			mergeChunks(k, chunk_counter - 1, chunk_counter);
			break;
		}	
		mergeChunks(k, k + 119, chunk_counter);
	}
	
	char filename[50];

	sprintf(filename, "chunk_%08d.tmp", chunk_counter - 1);
	int fdFinal = open(filename, O_RDONLY);
	fd = open(argv[1], O_WRONLY | O_TRUNC);

	std::vector<char> bufFinal;

	while(true){
		bufFinal = readLine(fdFinal);
	
		if(bufFinal.empty()){
			break;
		}

		int checkWrite = write(fd, bufFinal.data(), bufFinal.size());
		if(checkWrite < bufFinal.size()){
			int checkSeek = lseek(fd, -checkWrite, SEEK_CUR);
			if(checkSeek == -1){
				perror("lseek");
				exit(1);
			}

			checkSeek = lseek(fdFinal, -bufFinal.size(), SEEK_CUR); 
			if(checkSeek == -1){
				perror("lseek");
				exit(1);
			}

		}	
	}	
				
	close(fd);
	close(fdFinal);

	unlink(filename);

	if(readSize < 0){
		perror("read");
		return 1;
	}
		
	return 0;
}
