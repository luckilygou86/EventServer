/*
 * @Description: 
 * @version: 
 * @Author: sql
 * @Date: 2021-06-28 18:18:15
 * @LastEditors: sql
 * @LastEditTime: 2021-07-05 14:29:16
 */

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include "threadpools.h"
#include "glog_init.h"

// the constructor just launches some amount of workers
threadpools::threadpools(size_t threads) : size(threads), stop(false)
{
    VLOG(1) << "threadpools, threads:" << threads;
    for (size_t i = 0; i < threads; ++i)
    {
        queue_mutex.emplace_back(new std::mutex());
        tasks.emplace_back(std::queue<std::function<void()>>());
        condition.emplace_back(new std::condition_variable());
        workers.emplace_back(
            [this, i]() {
                std::function<void()> task;
                for (;;)
                {
                    {
                        std::unique_lock<std::mutex> lock(*this->queue_mutex[i]);
                        this->condition[i]->wait(lock,
                                                 [this, i] { return this->stop || !this->tasks[i].empty(); });
                        if (this->stop && this->tasks[i].empty())
                            return;
                        task = std::move(this->tasks[i].front());
                        this->tasks[i].pop();
                    }
                    task();
                }
            });
    }
}

// add new work item to the pool
bool threadpools::enqueue(std::function<void()> &&task, int idx)
{
    std::unique_lock<std::mutex> lock(*queue_mutex[idx]);
    tasks[idx].emplace(task);
    condition[idx]->notify_one();
    return true;
}

// the destructor joins all threads
threadpools::~threadpools()
{
    for (size_t i = 0; i < size; i++)
    {
        stop = true;
        condition[i]->notify_all();
        workers[i].join();
        delete condition[i];
        condition[i] = nullptr;
        delete queue_mutex[i];
        queue_mutex[i] = nullptr;
    }
}
