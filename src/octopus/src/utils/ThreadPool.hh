#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
	ThreadPool(uint32_t num_threads);
    void queueJob(const std::function<void()>& job);
    void stop();
    bool busy();

private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

	std::atomic_int working = 0;
};

void enqueue_and_wait(ThreadPool &pool_p, std::vector<std::function<void()>> const &jobs_p);

#endif
