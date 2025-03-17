#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>

int client_socket;
int rs;

void* reciever(void* args) {
  char buffer[1001];
      // Receive message from client
      while((rs = recv(client_socket, buffer, 1000, 0) > 0)) {
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
     std::cin >> mess;
     if (mess == 'exit') { break;}
     int sent = send(client_socket, mess.c_str(), mess.size(), 0);
     if(sent == -1){
       exit(errno);
     }
  }
  close(client_socket);
  return 0;
}
