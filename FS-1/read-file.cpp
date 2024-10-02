#include <iostream>
#include <fcntl.h>  
#include <unistd.h>
#include <string> 
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0];
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error" << argv[1] << ": " << strerror(errno) << std::endl;
        return 1;
    }

    const std::size_t buffer_size = 1024;
    char buffer[buffer_size];

    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
            std::cerr << "Error" << strerror(errno) << std::endl;
            close(fd);
            return 1;
        }
    }

    if (bytes_read == -1) {
        std::cerr << "Error" << argv[1] << ": " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        std::cerr << "Error" << argv[1] << ": " << strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}
