#include <iostream>

#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"
#include <string>
#include <sstream>

int main(int argc, char** argv) {

  int shm_fd = shm_open("/TransparentBank", O_RDWR, 0666);
  struct stat sb;
  if (fstat(shm_fd, &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
  }

  int shm_size = sb.st_size;
  std::cout << shm_size << std::endl; 
  void* shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  Bank* bank = (Bank*)shm_ptr;
  // bank->print();
  
  std::string list = std::string("choose command from the list \n") +
              "pcurb <id> - print current balance of ID \n" +
              "pminb <id> - print minimal balance of ID \n" +
              "pmaxb <id> - print maximal balance of <ID> \n" +
              "froze <ID> \n" +
              "defroze <ID> \n" +
              "tfromto <id> <id> <sum> - transfer from <ID> to <ID> <sum> \n" +
              "addall <sum> - enroll <sum> to all \n" +
              "suball <sum> - debit <sum> from all \n" +
              "sminb <id> <sum> - set minimal balance <sum> of <ID> \n" +
              "smaxb <id> <sum> - set maximal balance <sum> of <ID> \n" +
	      "exit \n"; 
               
  std::cout << list;
  while(true) {
     std::string command;
     std::getline(std::cin, command);
     if (command == "exit") {
        break;
     }
     
    if (command.find("pcurb" == 0)) {
       std::istringstream iss(command);
       std::string cmd;
       int id;

       iss >> cmd >> id;

       if (iss) {
        std::cout << "Current balance:\n " << bank->print_cur_balance(id);
       } else {
        std::cerr << "Ошибка: не удалось прочитать ID\n";
       }
       continue;
    } 

    if (command.find("pminb" == 0)) {
       std::istringstream iss(command);
       std::string cmd;
       int id;

       iss >> cmd >> id;  

       if (iss) {
        std::cout << "Minimal balance:\n " << bank->print_min_balance(id);
       } else {
        std::cerr << "Ошибка: не удалось прочитать ID\n";
       }
    }

    if (command.find("pmaxb" == 0)) {
       std::istringstream iss(command);
       std::string cmd;
       int id;

       iss >> cmd >> id;  

       if (iss) {
        std::cout << "Maximal balance:\n " << bank->print_max_balance(id);
       } else {
        std::cerr << "Ошибка: не удалось прочитать ID\n";
       }
    }
  
    if (command.find("froze" == 0)) {
       std::istringstream iss(command);
       std::string cmd;
       int id;

       iss >> cmd >> id;  

       if (iss) {
          bank->froze(id);
          std::cout << "Balance is frozen:\n ";
       } else {
        std::cerr << "Ошибка: не удалось прочитать ID\n";
       }
    }






}


  

  munmap(shm_ptr, shm_size);
  close(shm_fd);
	  
}

