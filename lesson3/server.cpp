#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

struct client {
   int socket;
   sockaddr_in address;
}

pthread_mutex_t mutex = PTHREAD_MUNEX_INITIALIZER;
std::vector<client*> clients; 

void send_to_all (char* message, int sender) {
    pthread_mutex_lock(&mutex);
    int i =0;
    while(i<clients.size()) {
       if (clients[i] == nullptr ||clients[i]->socket == sender) {
          continue;
       }
       send(cliens[i]->socket, message, strlen(message) + 1, 0);
       i++;
    }
    pthread_mutex_unlock(&mutex):
}


void* handle_client (void* arg) {
    client* client = (struct client*) arg;
    char buffer[3001];
      // Receive message from client
      while((rs = recv(client->socket, buffer, 1000, 0) > 0)) {
         buffer[rs] = '0';
         send_to_all(buffer, client->socket);
      }
      close(client_socket);
      delete client;
      client = nullptr;
      pthread_exit(NULL);
      
}

int main() {
  // create a socket  
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
      perror("socket creation error");
      exit(errno);
  }

  // create an endpoint
  
  // socket address
  struct sockaddr_in server_address;
  // internet protocol = AF_INET
  server_address.sin_family = AF_INET;
  // accept or any address (bind the socket to all available interfaces)
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  // port
  server_address.sin_port = htons(8888);

  // Bind server_socket to server_address
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
      perror("bind failed");
      exit(errno);
  }

  // Listen for incoming connections
  if (listen(server_socket, 24) < 0) {
    perror("listen failed");
    exit(errno);
  }
  std::cout << "Waiting for connection\n";

  while(true) {
      int client_socket;
      struct sockaddr_in client_address;
      unsigned int client_addr_len = sizeof(client_address);

      // Accept incoming connection
      if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
          perror("accept failed");
          exit(errno);
      }

      std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

      client *client = new struct client();
      client->socket = client_socket;
      client->address = client_address;
      
      pthread_mutex_lock(&mutex);
      clients.push_back(client); 
      pthread_mutex_unlock(&mutex);     

      pthread_t pthread;
      pthread_create(&thread, 0, handle_client, client);
      pthread_detach(threaad); 

  
  // close
  close(server_socket);
  return 0;
}
