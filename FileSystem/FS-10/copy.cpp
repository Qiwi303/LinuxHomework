#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <sys/stat.h>

void closed(const int fd1, const int fd2){
	if(close(fd1) == -1 || close(fd2) == -1){
		std::cerr<<"Failed to close"<<std::endl;
		exit(1);
	}
}

off_t copy_data(const off_t size, const int fd1, const int fd2) {
    char buf[4096];
    off_t remaining = size;
    off_t total_copied = 0;
    
    while (remaining > 0) {
        ssize_t to_read = (remaining < 4096) ? remaining : 4096;
        ssize_t bytes_read = read(fd1, buf, to_read);
        
        if (bytes_read == -1) {
            std::cerr << "Error: Failed to read from source file" << std::endl;
            closed(fd1, fd2);
	    exit(1);
        }
        if (bytes_read == 0) break;
        
        ssize_t bytes_written = write(fd2, buf, bytes_read);
        if (bytes_written == -1) {
            std::cerr << "Error: Failed to write to destination file" << std::endl;
            closed(fd1, fd2);
	    exit(1);
        }
        
        total_copied += bytes_written;
        remaining -= bytes_written;
    }
    
    return total_copied;
}

int main(int argc, char** argv){
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <source_file> <destination_file>" << std::endl;
        exit(1);
    }
    
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        std::cerr << "Error: Cannot open source file '" << argv[1] << "'" << std::endl;
        exit(1);
    }
    
    int fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (fd2 == -1) {
        std::cerr << "Error: Cannot open destination file '" << argv[2] << "'" << std::endl;
        if(close(fd1) == -1) {
		std::cerr<<"Failed to close"<<std::endl;
		exit(1);
        exit(1);
	}    
    }
    
    struct stat info;
    if (fstat(fd1, &info) == -1) {
        std::cerr << "Error: Cannot get file statistics" << std::endl;
        closed(fd1, fd2);
        exit(1);
    }
    
    off_t file_size = info.st_size;
    off_t logical_offset = 0;
    off_t total_data = 0;
    off_t total_holes = 0;
    
    while (logical_offset < file_size) {
        off_t data_offset = lseek(fd1, logical_offset, SEEK_DATA);
        
        if (data_offset == -1) {
            if (errno == ENXIO) {
                total_holes += file_size - logical_offset;
		int trunc = ftruncate(fd2, file_size);
		if(trunc == -1){
			std::cerr<<"Failed to trunc"<<std::endl;
			exit(1);
		}
                break;
            }
            std::cerr << "Error: lseek SEEK_DATA failed" << std::endl;
            close(fd1);
            close(fd2);
            return 1;
        }
        
        if (data_offset > logical_offset) {
            off_t hole_size = data_offset - logical_offset;
            total_holes += hole_size;
            
            if (lseek(fd2, data_offset, SEEK_SET) == -1) {
                std::cerr << "Error: lseek on destination failed" << std::endl;
                closed(fd1, fd2);
                exit(1);
            }
        }
        
        off_t hole_offset = lseek(fd1, data_offset, SEEK_HOLE);
        if (hole_offset == -1) {
            std::cerr << "Error: lseek SEEK_HOLE failed" << std::endl;
            closed(fd1, fd2);
            exit(1);
        }
        
        off_t data_size = hole_offset - data_offset;
        
        if (lseek(fd1, data_offset, SEEK_SET) == -1) {
            std::cerr << "Error: lseek SEEK_SET failed" << std::endl;
            closed(fd1, fd2);
            exit(1);
        }
        
        copy_data(data_size, fd1, fd2);
        total_data += data_size;
        
        logical_offset = hole_offset;
    }
    
   

	if (total_data + total_holes != file_size) {
  	  std::cerr << "Warning: data + holes != file_size" << std::endl;
  	  std::cerr << "Data: " << total_data << ", Holes: " << total_holes 
              	<< ", Sum: " << (total_data + total_holes) 
        	      << ", File size: " << file_size << std::endl;
	}

	closed(fd1,fd2);

	std::cout << "Successfully copied " << file_size << " bytes (data: " 
          << total_data << ", hole: " << total_holes << ")." << std::endl;		

    return 0;
}
