#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstring>

volatile sig_atomic_t sigusr2_count = 0;

void handle_sigusr1(int signum) {
   
}

// Обработчик для SIGUSR2
void handle_sigusr2(int signum) {
    sigusr2_count++; 
}

void handle_sigint(int signum) {
    std::cout << "Total SIGUSR2 received: " << sigusr2_count << std::endl;
    exit(0);
}

int main() {
    struct sigaction new_sa;
    memset(&new_sa, 0, sizeof(struct sigaction));
    new_sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &new_sa, nullptr);

    memset(&new_sa, 0, sizeof(struct sigaction));
    new_sa.sa_handler = handle_sigusr1;
    sigaction(SIGUSR1, &new_sa, nullptr);

    memset(&new_sa, 0, sizeof(struct sigaction));
    new_sa.sa_handler = handle_sigusr2;
    sigaction(SIGUSR2, &new_sa, nullptr);

    while (true) {
        std::cout << "I am still alive!!" << std::endl;
        sleep(5);
    }

    return 0;
}

