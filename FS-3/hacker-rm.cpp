#include <iostream>
#include <fcntl.h>   
#include <unistd.h> 
#include <cstring>  
#include <errno.h>  

int main(int argc, char** argv) {
    if (argc != 2) {
        return 1;
    }

    const char* path = argv[1];

    int fd = open(path, O_RDWR);
    if (fd == -1) {
        std::cerr << strerror(errno);
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        std::cerr << strerror(errno);
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        std::cerr << strerror(errno);
        close(fd);
        return 1;
    }

    char null = '\0';
    for (off_t i = 0; i < size; ++i) {
        if (write(fd, &null, 1) == -1) {
            std::cerr << strerror(errno);
            close(fd);
            return 1;
        }
    }

    if (close(fd) == -1) {
        std::cerr << strerror(errno);
        return 1;
    }

    if (unlink(path) == -1) {
        std::cerr << strerror(errno) ;
        return 1;
    }

    return 0;
}
