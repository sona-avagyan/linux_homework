#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

void do_command(char** argv) {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "fail to create a process" << std::endl;
        return;
    }

    if (pid == 0) {
        if (execvp(argv[0], argv) == -1) {
            std::cerr << "error of executing process " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            std::cout << "command ended with this code " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "error" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << argv[0] << std::endl;
        return 1;
    }

    do_command(argv + 1);
    return 0;
}
