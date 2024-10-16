#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "error " << argv[0] << std::endl;
        return 1;
    }

    const char* source_file = argv[1];
    const char* destination_file = argv[2];

    int fd_source = open(source_file, O_RDONLY);
    if (fd_source < 0) {
        std::cerr << "Error opening source file: " << strerror(errno) << std::endl;
        return 1;
    }

    int fd_destination = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fd_destination < 0) {
        std::cerr << "Error opening destination file: " << strerror(errno) << std::endl;
        close(fd_source);
        return 1;
    }

    const int buffer_size = 4096;
    char BUF[buffer_size];
    ssize_t all_bytes = 0;
    ssize_t all_data = 0;
    ssize_t all_holes = 0;
    
    off_t offset = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(fd_source, BUF, buffer_size)) > 0) {
        ssize_t cur_offset = 0;

        while (cur_offset < bytes_read) {
            off_t data_offset = lseek(fd_source, cur_offset + offset, SEEK_DATA);
            off_t hole_offset = lseek(fd_source, cur_offset + offset, SEEK_HOLE);

            if (data_offset == -1 || hole_offset == -1) {
                std::cerr << "Error seeking in source file: " << strerror(errno) << std::endl;
                close(fd_source);
                close(fd_destination);
                return 1;
            }

            if (data_offset > cur_offset + offset) {
                ssize_t hole_bytes = data_offset - (cur_offset + offset);
                all_holes += hole_bytes;
                lseek(fd_destination, hole_bytes, SEEK_CUR);
                cur_offset += hole_bytes;
            }

            if (hole_offset > data_offset) {
                ssize_t data_bytes = hole_offset - data_offset;
                ssize_t write_bytes = write(fd_destination, BUF + cur_offset, data_bytes);

                if (write_bytes < 0) {
                    std::cerr << "Error writing to destination file: " << strerror(errno) << std::endl;
                    close(fd_source);
                    close(fd_destination);
                    return 1;
                }

                all_bytes += write_bytes;
                all_data += write_bytes;
                cur_offset += data_bytes;
            }
        }
        offset += bytes_read;
    }

    if (bytes_read < 0) {
        std::cerr << "Error reading from source file: " << strerror(errno) << std::endl;
    }

    close(fd_source);
    close(fd_destination);

    std::cout << "Successfully copied " << all_bytes + all_holes << " bytes (data: " << all_data << ", hole: " << all_holes << ").\n";

    return 0;
}

