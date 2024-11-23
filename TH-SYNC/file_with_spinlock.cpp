#include <iostream>
#include <pthread.h>
#include <chrono>
// то,что в комментариях вообще должно работать, но у меня не работало, поэтому я загуглила))
class Integer {
private:
    int inner_value = 0;
   // pthread_spinlock_t spinlock;
public:
    Integer(int value) :inner_value(value) {
    	//pthread_spinlock_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
    }
    void inc() {
       // pthread_spin_lock(&spinlock);
        while (spinlock.test_and_set(std::memory_order_acquire));
        inner_value++;
	spinlock.clear(std::memory_order_release); 
       // pthread_spin_unlock(&spinlock);
    }
    int get_inner_value() {
        return inner_value;
    }
   // ~Integer(){
   //	pthread_spin_destroy(&spinlock);
   // }
};

void* increment(void* arg) {
    Integer* num = (Integer*)arg;
    for (int i = 0; i < 10000; ++i) {
        num->inc();
    }
    return nullptr;
}

int main(int argc, char** argv) {
    Integer* num = new Integer(0);
    int thread_count = 50;
    pthread_t* tids = new pthread_t[thread_count];

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&tids[i], NULL, increment, num);
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(tids[i], NULL);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> execution_time = end_time - start_time;

    std::cout << "Final value (spinlock): " << num->get_inner_value() << std::endl;
    std::cout << "Execution time: " << execution_time.count() << " seconds" << std::endl;

    delete[] tids;
    delete num;
    return 0;
}

