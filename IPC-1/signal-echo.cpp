#define _XOPEN_SOURCE 700
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ucontext.h> 

void handler(int, siginfo_t* info, void* context) {
    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;

    passwd* user_info = getpwuid(sender_uid);
    const char* username;
    if (user_info!=nullptr) {
        username = user_info->pw_name;
    } else {
        username = "unknown";
    }

    std::cout << "Received a SIGUSR1 signal from process " <<  sender_pid << "executed by " << sender_uid << " (" << username << ")" << std::endl;

    // честно говоря здесь не уверена, это загуглила))
    ucontext_t* uctx = static_cast<ucontext_t*>(context);
#if defined(__x86_64__)
    auto rip = uctx->uc_mcontext->__ss.__rip;
    auto rax = uctx->uc_mcontext->__ss.__rax;
    auto rbx = uctx->uc_mcontext->__ss.__rbx; 
    std::cout << "State of the context: RIP = " << std::hex << rip
              << ", RAX = " << std::hex << rax
              << ", RBX = " << std::hex << rbx << std::endl;
#elif defined(__i386__)
    auto eip = uctx->uc_mcontext.gregs[REG_EIP];
    auto eax = uctx->uc_mcontext.gregs[REG_EAX];
    auto ebx = uctx->uc_mcontext.gregs[REG_EBX];
    std::cout << "State of the context: EIP = " << std::hex << eip
              << ", EAX = " << std::hex << eax
              << ", EBX = " << std::hex << ebx << std::endl;
#else
    std::cout << "getting registers is nor available" << std::endl;
#endif
}

int main() {
    struct sigaction new_sa;
    new_sa.sa_sigaction = handler;  
    new_sa.sa_flags = SA_SIGINFO;         

    if (sigaction(SIGUSR1, &new_sa, nullptr) < 0) {
        perror("error");
        exit(1);
    }
    while (true) {
        sleep(10);
    }

    return 0;
}

