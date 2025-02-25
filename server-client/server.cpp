#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Client {
   int socket;
   sockaddr_in address;
};

void* handler (void* arg) {
   Client* client = (struct Client*) arg; 
   int client_socket = client->socket;
   sockaddr_in address = client->address;
   delete client;

   char buffer[1001];
   int rs;
   while ((rs = recv(client_socket, buffer, 1000, 0)) > 0) {
	buffer[rs] = '\0';
	pthread_mutex_lock(&mutex);
	std::cout << "message from " << inet_ntoa(address.sin_addr) << " " << buffer << std::endl;
	pthread_mutex_unlock(&mutex);
   }
   std::cout << "Client disconnected: " << inet_ntoa(address.sin_addr) << std::endl;
   close(client_socket);
   return nullptr;
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
  if (listen(server_socket, 10) < 0) {
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

      Client *client = new Client();
      client->socket = client_socket;
      client->address = client_address;
 
      pthread_t thread;
      pthread_create(&thread, nullptr, handler, client);
      pthread_detach(thread);
  }


  close(server_socket);
  pthread_mutex_destroy(&mutex);  
  return 0;
}
