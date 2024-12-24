#include "parallel_scheduler.hpp"
#include <iostream>
#include <unistd.h>
#include <pthread.h>

void example_function(int x) {
    usleep(500*1000);
    std::cout <<  x;
}

int main() {
    const size_t capacity = 4;
    const size_t num_tasks = 10;

    parallel_scheduler scheduler(capacity);

    for (int i = 0; i < num_tasks; i++) {
        scheduler.run(example_function, i);
    }

    usleep(6000*1000);

    return 0;
}
