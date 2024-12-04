#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <cstdlib>

int main() {
   pid_t pid = fork();
   
   if(pid == 0) {
      std::cout << "pid of child " << getpid();
      pid_t parent_pid = getpid();
      std::cout << "child is trying to kill its parent with pid " << parent_pid;
      if(kill(parent_pid, SIGKILL) == -1) {
         perror("parent is alive");
         exit(1);
      }
      exit(0);
   } else {
       std::cout << "pid of parent is " << getpid();
       wait(NULL);
   }

}
