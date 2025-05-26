#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "colorprint.hpp"

constexpr int PORT = 8888;

std::atomic<int> request_count{0};
std::mutex count_mutex;
std::condition_variable count_cond;
bool shutdown_requested = false;

// Поток для вывода каждых 5 запросов
void monitor_requests() {
    int last_reported = 0;
    std::unique_lock<std::mutex> lock(count_mutex);
    while (!shutdown_requested) {
        count_cond.wait(lock, [&]() {
            return request_count - last_reported >= 5 || shutdown_requested;
        });

        if (shutdown_requested) break;

        last_reported = request_count.load();
        Painter p(std::cout, {"REQUESTS"}, {});
        p << "=== REQUESTS processed: " + std::to_string(last_reported) + " ===\n";
    }
}

// Поток для клиента
void handle_client(int client_socket) {
    char buffer[1001];
    ssize_t bytes_received = recv(client_socket, buffer, 1000, 0);
    if (bytes_received <= 0) {
        close(client_socket);
        return;
    }

    buffer[bytes_received] = '\0';
    std::string msg(buffer);

    Painter p(std::cout, {"shutdown", "ADD", "SUB", "MUL", "DIV"}, {"ERROR", "FAIL"});
    p << "[Client] Message received: " + msg + "\n";

    if (msg.find("shutdown") != std::string::npos) {
        shutdown_requested = true;
        count_cond.notify_all();
        send(client_socket, "Server shutting down.\n", 23, 0);
    } else {
        // Имитация вычисления
        std::string reply = "Result: OK\n";
        send(client_socket, reply.c_str(), reply.size(), 0);
        request_count++;
        count_cond.notify_all();
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket error");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        perror("listen error");
        return 1;
    }

    std::cout << "Server is running on port " << PORT << "\n";

    std::thread stats_thread(monitor_requests);

    while (!shutdown_requested) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            if (shutdown_requested) break;
            perror("accept error");
            continue;
        }

        std::thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    stats_thread.join();
    close(server_socket);
    std::cout << "Server shut down gracefully.\n";
    return 0;
}

