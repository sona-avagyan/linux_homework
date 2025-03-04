#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>

int client_socket;
int rs = 0;

void* reciever(void* args) {
  char buffer[1001];
      // Receive message from client
      while((rs = recv(client_socket, buffer, 1000, 0)) > 0) {
         buffer[rs] = '0';
         std::cout << buffer << "\n";
      }
      close(client_socket);
      exit(1);
}

int main(){

  // server address
  struct sockaddr_in server_address;

  // create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
        perror("socket creation error");
        exit(errno);
  }

  // fill the server ip and port address
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);

  // connect
  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(connected == -1){
    perror("connection failed");
    exit(errno);
  }

  pthread_t thread;
  pthread_create(&thread, 0, reciever, NULL);
  pthread_detach(thread);
  // send message to server
  std::string mess;
  while (true) {
     std::getline(std::cin, mess);
     if (mess == "exit") { 
       send(client_socket, (mess + "\n").c_str(), mess.size()+1, 0);
       close(client_socket);
     }
     send(client_socket, (mess + "\n").c_str(), mess.size() + 1, 0);
  } 
  return 0;
}
