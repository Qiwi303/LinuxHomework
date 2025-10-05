#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<vector>
#include<queue>

struct comporator {
	bool operator()(std::pair<int, std::vector<char>> i, std::pair<int, std::vector<char>> j) {
		return i.second < j.second;
	}
};

void bubbleSort(std::vector<std::vector<char>>& lines){
	for(int i = 0; i < lines.size(); ++i){
		for(int j = 0; j < lines.size() - i - 1; ++j){
			if(lines[j] > lines[j+1]){
				std::swap(lines[j], lines[j+1]);
			}
		}
	}
}


void printVec(const std::vector<std::vector<char>>& lines){
	for(auto x: lines){
		for(char y:x){
			std::cout<<y;
		}
	}
	std::cout<<std::endl;
}


//TODO: extract readLine method add while(true) after every iteration write the line to the fdMerged read newline from corresponding fd(if fd is empty read from next fd if exists) if all fds are empty then close all the fds and fdMerged, then unlink them but fdMerged, and run mergeChunks for the next files. When all the files are merged and you have exactly one file write all the info from tmp to your initial file(O_TRUNC). but before just  check them.
void mergeChunks(int start, int end, int &current_chunk) {
	std::priority_queue(std::pair<int, std::vector<char>>, std::vector<std::pair<int,std::vector<char>>>, comporator) minHeap;
	std::vector<int> fds;
	char filename[50];
	for(int i = start; i <= end; ++i) { 
		sprintf(filename, "chunk_%08d.tmp", start);
	
		int fd = open(filename, O_RDWR, 0600);
		fds.push_back(fd);
		start++;
	}
	sprintf(filename, "chunk_%08d.tmp", current_chunk);

	int fdMerged = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
	
	std::vector<char> buf; 
	for(int i = start; i <= end; ++i) {
		char x;
		while(true){
			int readBytes = read(fds[i], x, 1);

			buf.push_back(x);
		
			if(readBytes == -1) {
		 		perror("read");
				return 1;
			}
			if(readBytes == 0) {

			}

			if(int(buf[buf.size()-1]) == int('\n')) {
				minHeap.push_back(std::pair<int, std::vector<char>>(i, buf);
				buf.clear();
				break;
			}

		}
	}
		
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
	int sorted_chunk_counter = 0;	

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
	std::cout << "read: " << readSize << std::endl;
	std::cout << "last_nl: " << last_newline_pos << std::endl;
	std::cout << "btr: " << bytes_to_rewind << std::endl;

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

	bubbleSort(lines);
	
	
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

	if(readSize < 0){
		perror("read");
		return 1;
	}
		
	return 0;
}
	
