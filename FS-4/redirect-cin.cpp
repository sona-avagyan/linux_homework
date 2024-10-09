#include <iostream>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <cstring>

void initialize(int argc, char** argv){
	if (argc!=2){
		std::cerr << argv[0];
		exit(1);
	}	

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		std::cerr << argv[1];
		exit(2);
	}

	if(dup2(fd, STDIN_FILENO)<0){
		std::cerr << "Error";
		close(fd);
		exit(3);
	}	
	close(fd);
}

int main(int argc, char** argv){
	initialize(argc, argv);
	std::string str1;
	std::cin >> str1;
	std::string str2;
	for(int i = str1.size()-1; i>=0; --i){
		str2 +=str1[i];
	}
	std::cout << str2 << std::endl;
}

