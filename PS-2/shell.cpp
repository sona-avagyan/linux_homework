#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <cstring>

void addCurrentDirToPath(char **envp) {
    std::string path;
    for (int i = 0; envp[i] != nullptr; i++) {
        std::string var(envp[i]);
        if (var.find("PATH=") == 0) {
            path = var.substr(5); 
            break;
        }
    }

    path = "./:" + path;

    std::string new_path = "PATH=" + path;
    putenv(strdup(new_path.c_str()));
}

std::vector<std::string> split(const std::string& str) {
    std::istringstream stream(str);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int executeCommand(const std::vector<std::string>& args, bool silentMode, const std::string& outputFile = "", bool append = false) {
    pid_t pid = fork();
    if (pid == 0) {
        int fd = -1;
        if (!outputFile.empty()) {
            fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        } else if (silentMode) {
            std::string logFile = std::to_string(getpid()) + ".log";
            fd = open(logFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        std::vector<char*> execArgs;
        for (const auto& arg : args) execArgs.push_back(const_cast<char*>(arg.c_str()));
        execArgs.push_back(nullptr);
        execvp(execArgs[0], execArgs.data());
        perror("execvp failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void parseAndExecute(std::string input) {
    bool silentMode = false;
    if (input.substr(0, 6) == "silent") {
        silentMode = true;
        input = input.substr(7);
    }

    std::istringstream cmdStream(input);
    std::string sign;
    std::string output;
    bool append = false;
    int lastExitStatus = 0;
    std::vector<std::string> args;

    while (cmdStream >> sign) {
        if (sign == ";") {
            lastExitStatus = executeCommand(args, silentMode, output, append);
            args.clear();
            output.clear();
            append = false;
        } else if (sign == "&&") {
            if (lastExitStatus == 0) {
                lastExitStatus = executeCommand(args, silentMode, output, append);
            }
            args.clear();
            output.clear();
            append = false;
        } else if (sign == "||") {
            if (lastExitStatus != 0) {
                lastExitStatus = executeCommand(args, silentMode, output, append);
            }
            args.clear();
            output.clear();
            append = false;
        } else if (sign == ">") {
            cmdStream >> output;
            append = false;
        } else if (sign == ">>") {
            cmdStream >> output;
            append = true;
        } else {
            args.push_back(sign);
        }
    }

    if (!args.empty()) {
        executeCommand(args, silentMode, output, append);
    }
}


int main(int argc, char** argv, char** envp){
    addCurrentDirToPath(envp);
    while (true) {
        std::cout << "interactive-shell> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit"){
            break;
        }
        if (!input.empty()) {
            parseAndExecute(input);
        }
    }
    return 0;	
}
