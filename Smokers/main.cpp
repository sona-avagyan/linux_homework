#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>
#include <cstring>

sem_t *sem_barman_ready, *sem_smoker_t, *sem_smoker_p, *sem_smoker_m, *sem_smoking_done;

void smoker(char smth, const char* sem_name, char output_char) {
    sem_t *sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    while (true) {
        sem_wait(sem);
        std::cout << output_char <<std::endl;
        sem_post(sem_smoking_done); 
    }

    sem_close(sem);
    exit(0);
}

void barman() {
    char smth;
    while (std::cin >> smth) {
        sem_wait(sem_barman_ready);

        switch (smth) {
            case 't':
                sem_post(sem_smoker_t);
                break;
            case 'p':
                sem_post(sem_smoker_p);
                break;
            case 'm':
                sem_post(sem_smoker_m);
                break;
        }

        sem_wait(sem_smoking_done);
    }

    kill(0, SIGTERM);
    exit(0);
}

int main() {
    sem_unlink("/sem_barman_ready");
    sem_unlink("/sem_smoker_t");
    sem_unlink("/sem_smoker_p");
    sem_unlink("/sem_smoker_m");
    sem_unlink("/sem_smoking_done");

    sem_barman_ready = sem_open("/sem_barman_ready", O_CREAT, 0644, 1);
    sem_smoker_t = sem_open("/sem_smoker_t", O_CREAT, 0644, 0);
    sem_smoker_p = sem_open("/sem_smoker_p", O_CREAT, 0644, 0);
    sem_smoker_m = sem_open("/sem_smoker_m", O_CREAT, 0644, 0);
    sem_smoking_done = sem_open("/sem_smoking_done", O_CREAT, 0644, 0);

    if (sem_barman_ready == SEM_FAILED || sem_smoker_t == SEM_FAILED ||
        sem_smoker_p == SEM_FAILED || sem_smoker_m == SEM_FAILED || sem_smoking_done == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    if (fork() == 0) {
	 smoker('t', "/sem_smoker_t", 'T');
         smoker('p', "/sem_smoker_p", 'P');
         smoker('m', "/sem_smoker_m", 'M');
    }

    barman();

    sem_close(sem_barman_ready);
    sem_close(sem_smoker_t);
    sem_close(sem_smoker_p);
    sem_close(sem_smoker_m);
    sem_close(sem_smoking_done);

    sem_unlink("/sem_barman_ready");
    sem_unlink("/sem_smoker_t");
    sem_unlink("/sem_smoker_p");
    sem_unlink("/sem_smoker_m");
    sem_unlink("/sem_smoker_done");

    return 0;
}

