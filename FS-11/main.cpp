#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv){
    std::string source_file_name;
    std::cout << "input the first file name \n"<<std::endl;
    std::cin >> source_file_name;

    std::string dest_file_name;
    std::cout << "input the second file name \n"<<std::endl;
    std::cin >> dest_file_name;

    int first_offset, second_offset, size;

    std::cout << "first offset \n"<<std::endl;
    std::cin >> first_offset;

    std::cout << "second offset \n"<<std::endl;
    std::cin >> second_offset;

    std::cout << "size \n"<<std::endl;
    std::cin >> size;

    int fd_source = open(source_file_name.c_str(), O_RDONLY);
    if(fd_source < 0) {
      std::cerr << "error opening source file" << strerror(errno) << std::endl;
      return 1;
    }

    int fd_destination = open(dest_file_name.c_str(), O_WRONLY | O_CREAT, 0644);
    if(fd_destination < 0){
      std::cerr << "error opening destination file" << strerror(errno) << std::endl;
      close(fd_source);
      return 1;
    }

    if(lseek(fd_source, first_offset, SEEK_SET) == (off_t)-1){
      std::cerr << "error seeking in source file" << strerror(errno) << std::endl;
      close(fd_source);
      close(fd_destination);
      return 1;
    }

    const int buffer_size = 4096;
    char BUF[buffer_size];
    int all_bytes;

    while(all_bytes < size){
      int bytes_to_read = std::min(buffer_size, size - all_bytes);
      ssize_t bytes_read = read(fd_source, BUF, bytes_to_read);
      if(bytes_read < 0){
        std::cerr << "error reading source file" << strerror(errno) << std::endl;
        close(fd_source);
        close(fd_destination);
        return 1;
      } else if (bytes_read == 0) {
          break;
        }
      
      ssize_t bytes_write = write(fd_destination, BUF, bytes_read);
      if(bytes_write < 0){
        std::cerr << "error writing in destination file" << strerror(errno) << std::endl;
        close(fd_source);
        close(fd_destination);
        return 1;
      }
      all_bytes+= bytes_write;
    }
    close(fd_source);
    close(fd_destination);

    std::cout << "Successfully copied " << size << " bytes from the source file to the destination file.\n";
}
