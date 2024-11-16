#include <iostream>
#include <vector>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

struct ThreadData {
    const std::vector<int>* array;
    size_t start;
    size_t end;
    long long result;
};

void* partial_sum(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    data->result = 0;
    for (size_t i = data->start; i < data->end; i++) {
        data->result += (*data->array)[i];
    }
    return nullptr;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << argv[0];
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    size_t M = std::stoull(argv[2]);

    std::vector<int> array(N);
    for (size_t i = 0; i < N; i++) {
        array[i] = rand() % 100;
    }

    long long single_thread_sum = 0;
    clock_t start_time = clock();
    for (size_t i = 0; i < N; i++) {
        single_thread_sum += array[i];
    }
    clock_t end_time = clock();
    std::cout << single_thread_sum << "\n";
    std::cout << "Time spent without threads: " << 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC << " ms\n";

    std::vector<ThreadData> thread_data(M);
    std::vector<pthread_t> threads(M);
    size_t partial_size = N / M;
    start_time = clock();
    
    for (size_t i = 0; i < M; i++) {
        thread_data[i].array = &array;
        thread_data[i].start = i * partial_size;
        if(i == M-1){
	    thread_data[i].end = N;
	} else {
	    thread_data[i].end = (i+1) * partial_size;
	}
        pthread_create(&threads[i], nullptr, partial_sum, &thread_data[i]);
    }

    long long multi_thread_sum = 0;
    for (size_t i = 0; i < M; i++) {
        pthread_join(threads[i], nullptr);
        multi_thread_sum += thread_data[i].result;
    }
    
    end_time = clock();
    std::cout << multi_thread_sum << "\n";
    std::cout << "Time spent with " << M << " threads: " << 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC << " ms\n";

    return 0;
}

