#include <iostream>   
#include <fcntl.h>    
#include <unistd.h>  
#include <cstring>   
#include <errno.h>

int main (int argc, char**  argv){
  if (argc !=3){
    std::cerr << argv[0];
    return 1;
  }

  int fd1 = open(argv[1], O_RDONLY);
  if(fd1 == -1){
    std::cerr << argv[1]<< strerror(errno);
    return 1; 
  }

  int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (fd2 == -1){
    std::cerr << argv[2] << strerror(errno);
    close(fd1);
    return 1;
  }
  
  const std::size_t  buf_size = 4096;
  char BUF[buf_size];
  int bytes_read;
  while((bytes_read = read (fd1, BUF, buf_size))>0) {
    if(write(fd2, BUF, bytes_read) != bytes_read) {
      std::cerr << argv[2] << strerror(errno);
      close(fd1);
      close(fd2);
      return 1;
    }
  }
  
  if (bytes_read == -1) {
    std::cerr << argv[1] <<strerror(errno);
    close(fd1);
    close(fd2);
    return 1;
  }
  
  if (close(fd1) == -1) {
    std::cerr << argv[1] << strerror(errno);
    return 1;
  }

  if (close(fd2) == -1) {
    std::cerr << argv[2] << strerror(errno);
    return 1;
  }
}
