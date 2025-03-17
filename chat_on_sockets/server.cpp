// честно говоря не получилось нормально реализовать момент с exit и list
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

struct Client {
   int socket;
   sockaddr_in address;
   std::string name;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<Client*> clients;

void send_to_all (const std::string& message, int sender) {
    pthread_mutex_lock(&mutex);
    int i =0;
    while(i<clients.size()) {
       if (clients[i] == nullptr ||clients[i]->socket == sender) {
          continue;
       }
       send(clients[i]->socket, message.c_str(), message.size(), 0);
       i++;
    }
    pthread_mutex_unlock(&mutex);
}

int rs = 0;
void* handle_client (void* arg) {
      Client* client = (struct Client*) arg;
      char buffer[3001];
      // Receive message from client
      buffer[rs] = '\0';
      client->name = buffer;

      std::string welcome_msg = client->name + " joined the chat!\n";
      send_to_all(welcome_msg, client->socket);
      while((rs = recv(client->socket, buffer, 1000, 0) > 0)) {
         buffer[rs] = '0';
         std::string msg = buffer;

        if (msg == "/exit\n") {
            break;
        } else if (msg == "/list\n") {
            std::string list = "Online users:\n";
            pthread_mutex_lock(&mutex);
            int i = 0;
            while (i<clients.size()) {
                if (clients[i]) list += clients[i]->name + "\n";
            }
            pthread_mutex_unlock(&mutex);
            send(client->socket, list.c_str(), list.size(), 0);
        } else {
            send_to_all(client->name + ": " + msg + "\n", client->socket);
        }
      }
      std::string leave_msg = client->name + " left the chat.\n";
      send_to_all(leave_msg, client->socket);
      
      close(client->socket);

      pthread_mutex_lock(&mutex);
      clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
      pthread_mutex_unlock(&mutex);
      
      delete client;
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

      Client *client = new struct Client();
      client->socket = client_socket;
      client->address = client_address;

      pthread_mutex_lock(&mutex);
      clients.push_back(client);
pthread_mutex_unlock(&mutex);

      pthread_t thread;
      pthread_create(&thread, 0, handle_client, client);
      pthread_detach(thread);

}
  // close
  close(server_socket);
  return 0;
}
