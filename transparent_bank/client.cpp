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
  struct sockaddr_in server_address;
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
        perror("socket creation error");
        exit(errno);
  }
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);
  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(connected == -1){
    perror("connection failed");
    exit(errno);
  }




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
    std::string line;
    std::getline(std::cin, line);
     
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "exit") {
	std::cout << "Disconnect" << std::endl;

    } else if (cmd == "pcurb") {
       int id;
       iss >> id;

       if (!iss.fail()) {
         bank->print_cur_balance(id);
       } else {
         std::cerr << "Ошибка: не удалось прочитать ID\n";
       }

    } else if (cmd == "pminb") {
       int id;
       iss >> id;  

       if (!iss.fail()) {
         bank->print_min_balance(id);
       } else {
         std::cerr << "Ошибка: не удалось прочитать ID\n";
       }

    } else if (cmd == "pmaxb") {
       int id;
       iss >> id;  

       if (!iss.fail()) {
         bank->print_max_balance(id);
       } else {
         std::cerr << "Ошибка: не удалось прочитать ID\n";
       }
    }
    
   // if (command.find("froze")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int id;

   //    iss >> cmd >> id;  

   //    if (iss) {
   //       bank->froze(id);
   //       std::cout << "Balance is frozen:\n ";
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }

   // if (command.find("defroze")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int id;

   //    iss >> cmd >> id;  

   //    if (iss) {
   //       bank->defroze(id);
   //       std::cout << "Balance is defrozen:\n ";
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }

   // if (command.find("tfromto")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int id1, id2;
   //    int sum;

   //    iss >> cmd >> id1 >> id2 >> sum;  

   //    if (iss) {
   //       bank->transfer(id1, id2, sum);
   //       std::cout << sum << "Transfered from" << id1 << "to" << id2;
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }

   // if (command.find("addall")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int sum;

   //    iss >> cmd >> sum;

   //    if (iss) {
   //       bank->enroll_to_all(sum);
   //       std::cout << sum << "Transfered to all accounts";
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }

   // if (command.find("suball")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int sum;

   //    iss >> cmd >> sum;

   //    if (iss) {
   //       bank->debit_from_all(sum);
   //       std::cout << sum << "Transfered from all accounts";
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }

   // if (command.find("setminb")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int id;
   //    int sum;

   //    iss >> cmd >> id >> sum;

   //    if (iss) {
   //       bank->set_min_balance(id, sum);
   //       std::cout << "Your minimal balance set to " << sum;
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }

   // if (command.find("setmaxb")) {
   //    std::istringstream iss(command);
   //    std::string cmd;
   //    int id;
   //    int sum;

   //    iss >> cmd >> id >> sum;

   //    if (iss) {
   //       bank->set_max_balance(id, sum);
   //       std::cout << "Your maximal balance set to " << sum;
   //    } else {
   //     std::cerr << "Ошибка: не удалось прочитать ID\n";
   //    }
   // }
}


  
  close(client_socket);
  munmap(shm_ptr, shm_size);
  close(shm_fd);
	  
}

