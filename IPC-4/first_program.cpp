#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

int main() {
    int signal_number, pid;

    std::cout << "Enter signal number: ";
    std::cin >> signal_number;
    std::cout << "Enter PID: ";
    std::cin >> pid;

    if (kill(pid, signal_number) == -1) {
        perror("Error sending signal");
        return 1;
    }

    std::cout << "Signal " << signal_number << " sent to process " << pid << std::endl;
    return 0;
}

