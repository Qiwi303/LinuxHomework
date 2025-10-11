#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>

#define BUFFSIZE 4096

void removeFileData(const int fd, int size){
        char buf[BUFFSIZE] = {'\0'};
	int written;
        while(true){
		if(size >= BUFFSIZE){
			written = write(fd, buf, BUFFSIZE);
		}
		else{
			written = write(fd, buf, size);
			size = 0;
		}	break;

                if(written == -1){
                std::cerr<<"failed to write"<<std::endl;
                        exit(1);
                }

                size-=BUFFSIZE;
        }
	int check = ftruncate(fd, 0);
	if(check == -1){
		std::cerr<<"failed to ftruncate"<<std::endl;
		exit(1);
	}

}



int main(int argc, char** argv){
        if(argc != 2){
                std::cerr<<"invalid arguments"<<std::endl;
                exit(1);
        }

        int fd = open(argv[1], O_WRONLY);
        if(fd == -1){
                std::cerr<<"failed to open"<<std::endl;
                exit(1);
        }



        struct stat info;
        int statCheck = stat(argv[1], &info);
        if(statCheck == -1){
                std::cerr<<"failed to get stat"<<std::endl;
                exit(1);
        }


        if(!S_ISREG(info.st_mode)){
                std::cerr<<"File isnt regular"<<std::endl;
                exit(1);
        }

        int size = info.st_size;

        if(size == 0){
                if(close(fd) == -1){
                        std::cerr<<"failed to close file"<<std::endl;
                        exit(1);
                }

                int unlinked = unlink(argv[1]);
                if(unlinked  == -1){
                        std::cerr<<"failed to delete file"<<std::endl;
                        exit(-1);
                }
                return 0;
        }

        removeFileData(fd, size);

        int closed = close(fd);
        if(closed == -1){
                std::cerr<<"failed to close"<<std::endl;
        }

        //int unlinked = unlink(argv[1]);
        //if(unlinked == -1){
        //      std::cerr<<"failed to unlink"<<std::endl;
        //      exit(1);
        //}

        return 0;
}
