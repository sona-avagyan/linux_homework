#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

bool is_prime(int num) {
	if(num<=1) {
		return false;
	}
	for(int i = 2; i*i<=num; i++){
		if(num%i == 0) {
			return false;
		}
	}
	return true;
}

int calculate_mth_prime_number(int m) {
	int i =0;
	int num = 2;
	while(i<m) {
		if(is_prime(num)) {
			i++;
		}
		num++;
	}
	return num-1;
}

int main (int argc, char** argv) {
	int pipe_p_c[2], pipe_c_p[2];
	if(pipe(pipe_p_c) == -1 || pipe(pipe_c_p) == -1) {
		return 1;
	}
	
	pid_t pid = fork();
	if(pid<0) {
		return 1;
	}

	if(pid>0){
		close(pipe_p_c[0]);
		close(pipe_c_p[1]);

	while(true) {
		std::string input;
		std::cout << "[Parent] Please enter the number:";
		std::cin>>input;

		if(input=="exit"){
			std::cout << "[Parent] Exiting";
			int exit_signal = -1;
                	write(pipe_p_c[1], &exit_signal, sizeof(exit_signal));
			break;
		}
		int m;
		try {
                	m = std::stoi(input);
            	} catch (const std::invalid_argument&) {
                	std::cout << "[Parent] Invalid input";
                	continue;
            	}	

		std::cout<< "[Parent] Sending " << m << " to the child process...";
		write(pipe_p_c[1], &m, sizeof(m));

		std::cout << "[Parent] Waiting for the response from the child process...";
		int res;
		read(pipe_c_p[0], &res, sizeof(res));
		std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << res << "..." ;
	}
	close(pipe_p_c[1]);
	close(pipe_c_p[0]);
	wait(NULL);
	} else {
		close(pipe_p_c[1]);
		close(pipe_c_p[0]);

		int m;
		while(true) {
			read(pipe_p_c[0], &m, sizeof(m));
			if (m == -1) {
                		std::cout << "[Child] Exiting...";
                		break;
            		}
			std::cout << "[Child] Calculating " << m << "-th prime number...";
			int res = calculate_mth_prime_number(m);
			std::cout << "[Child] Sending calculation result of prime(" << m << ")...";
			write(pipe_c_p[1], &res, sizeof(res));
		}
	close(pipe_p_c[0]);
	close(pipe_c_p[1]);
	}
}
