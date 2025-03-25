#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstring>
#include <poll.h>
#include <vector>
#include <queue>
#include <sstream>

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;
std::queue<int> client_queue;

std::string operations(const std::string &request) {
    std::istringstream iss(request);
    std::string operation;
    double num1, num2, result;
    
    if (!(iss >> operation >> num1 >> num2)) {
        return "ERROR: Invalid command format";
    }
    
    if (operation == "ADD") {
        result = num1 + num2;
    } else if (operation == "SUB") {
        result = num1 - num2;
    } else if (operation == "MUL") {
        result = num1 * num2;
    } else if (operation == "DIV") {
        if (num2 == 0) return "ERROR: Division by zero";
        result = num1 / num2;
    } else {
        return "ERROR: Unknown operation";
    }
    
    return std::to_string(result);
}

void *worker_thread(void *arg) {
  int client_socket = *(int *)arg;
  delete (int*)arg;

  while (true) {
    char buffer[1024] = {0};
    int rs = recv(client_socket, buffer, 1023, 0);
    if (rs <= 0) {
       if (rs == 0) {
          std::cout << "Client disconnected\n";
       }  else {
          perror("recv error");
       }   
       break;
    } else {
      buffer[rs] = '\0';
      std::string response = operations(buffer);
      send(client_socket, response.c_str(), response.length(), 0);
    }
  }
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

  struct pollfd fds[10];
  fds[0].fd = server_socket;
  fds[0].events = POLLIN;

  int client_count = 1;

  while (true) {
    int poll_count = poll(fds, client_count, -1);
    if (poll_count < 0) {
       perror("poll error");
       continue;
    }

    if (fds[0].revents & POLLIN) {
       struct sockaddr_in client_addr;
       socklen_t client_len = sizeof(client_addr);
       int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
       if (client_socket >= 0) {
          std::cout << "New client connected\n";
          fds[client_count].fd = client_socket;
          fds[client_count].events = POLLIN;
          client_count++;

          pthread_t thread;
          int* client_socket_ptr = new int(client_socket);
          pthread_create(&thread, nullptr, worker_thread, client_socket_ptr);
          pthread_detach(thread);
       }
    }

    for (int i = 1; i < client_count; i++) {
        if (fds[i].revents & POLLIN) {
        char buffer[1024] = {0};
        int client_socket = fds[i].fd;
        int rs = recv(client_socket, buffer, 1023, 0);
        if (rs > 0) {
           buffer[rs] = '\0';
           std::string response = operations(buffer);
           send(client_socket, response.c_str(), response.length(), 0);
        } else if (rs == 0) {
           std::cout << "Client disconnected\n";
           close(client_socket);
           fds[i] = fds[--client_count];
        } else {
           perror("recv error");
           close(client_socket);
           fds[i] = fds[--client_count]; 
        }
        }
    }
  }

  // close
  close(server_socket);
  return 0;
}
