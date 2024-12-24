#ifndef PARALLEL_SCHEDULER_HPP
#define PARALLEL_SCHEDULER_HPP

#include <pthread.h>
#include <queue>
#include <vector>
#include <functional>
#include <atomic>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity)
        : max_threads(capacity), stop(false) {
        pthread_mutex_init(&queue_mutex, nullptr);
        pthread_cond_init(&queue_cv, nullptr);

        threads.resize(max_threads);
        for (size_t i = 0; i < max_threads; ++i) {
            pthread_create(&threads[i], nullptr, thread_entry, this);
        }
    }

    ~parallel_scheduler() {
        {
            pthread_mutex_lock(&queue_mutex);
            stop = true;
            pthread_cond_broadcast(&queue_cv);
            pthread_mutex_unlock(&queue_mutex);
        }

        for (pthread_t& thread : threads) {
            pthread_join(thread, nullptr);
        }

        pthread_mutex_destroy(&queue_mutex);
        pthread_cond_destroy(&queue_cv);
    }

    template <typename Function, typename... Args>
    void run(Function&& func, Args&&... args) {
    	auto task = [func, args...]() {
        func(args...);
    };

    pthread_mutex_lock(&queue_mutex);
    task_queue.emplace(std::move(task));
    pthread_cond_signal(&queue_cv);
    pthread_mutex_unlock(&queue_mutex);
}

private:
    static void* thread_entry(void* arg) {
        auto* scheduler = static_cast<parallel_scheduler*>(arg);
        scheduler->worker_thread();
        return nullptr;
    }

    void worker_thread() {
        while (true) {
            std::function<void()> task;

            pthread_mutex_lock(&queue_mutex);
            while (!stop && task_queue.empty()) {
                pthread_cond_wait(&queue_cv, &queue_mutex);
            }

            if (stop && task_queue.empty()) {
                pthread_mutex_unlock(&queue_mutex);
                return;
            }

            task = std::move(task_queue.front());
            task_queue.pop();
            pthread_mutex_unlock(&queue_mutex);

            task();
        }
    }

    size_t max_threads;
    std::vector<pthread_t> threads;
    std::queue<std::function<void()>> task_queue;

    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cv;

    std::atomic<bool> stop;
};

#endif // PARALLEL_SCHEDULER_HPP

