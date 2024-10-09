#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv){
	if(argc!=2){
		std::cerr << "error" << std::endl;
		return 1;
	}
	
	int fd1 = open(argv[1], O_WRONLY | O_CREAT, 0644);
	if(fd1 < 0){
		std::cerr << "error" << std::endl;
		return 1;
	}	
	
	int fd2 = dup(fd1);
	if(fd2 < 0){
                std::cerr << "error" << std::endl;
		close(fd2);
                return 1;
        }

	const char* line1 = "first line\n";
	if(write(fd1, line1, 11)<0){
		std::cerr << "error" << std::endl;
		close(fd1);
		close(fd2);
		return 1;
	}

	const char* line2 = "second line\n";
        if(write(fd2, line2, 12)<0){
                std::cerr << "error" << std::endl;
                close(fd1);
                close(fd2);
                return 1;
        }

	close(fd1);
	close(fd2);
}
