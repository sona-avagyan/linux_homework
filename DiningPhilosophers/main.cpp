#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>

std::vector<sem_t*> sticks(5);

void think(int id) {
	std::cout << id << "th is thinking";
	usleep(rand() % 1000000);
}

void eat(int id) {
	std::cout << id << "th is eating";
	usleep(rand() % 1000000);
}

void philosopher (int id) {
	int left = id;
	int right = (id + 1) % 5;
	if (id == 0) {
		std::swap(left, right);
	} 
	
	for (int i =0; i<10; i++) {
		think(id);
		sem_wait(sticks[left]);
		sem_wait(sticks[right]);
		eat(id);
		sem_post(sticks[right]);
		sem_post(sticks[left]);
	}
	
	exit(0);
}

int main() {
	srand(time(nullptr));

	for (int i = 0; i<5; i++) {
		std::string name = "/stick" + std::to_string(i);
		sticks[i] = sem_open(name.c_str(), O_CREAT, 0644, 1);
		if (sticks[i] == SEM_FAILED) {
			perror("sem_open failed");
			exit(EXIT_FAILURE);
		}
	}

	std::vector<pid_t> pids;
	for (int i =0; i<5; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			philosopher(i);
		} else if (pid>0) {
			pids.push_back(pid);
		} else {
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
	}

	for (size_t i = 0; i < pids.size(); i++) {
    		waitpid(pids[i], nullptr, 0);
	}


	for (int i =0; i<5; i++) {
		sem_close(sticks[i]);
		std::string name = "/stick" + std::to_string(i);
		sem_unlink(name.c_str());
	}
}

