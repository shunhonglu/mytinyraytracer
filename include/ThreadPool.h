#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>

class ThreadPool {
public:
    void push_back(std::thread thr) { m_pool.push_back(std::move(thr)); }

    ~ThreadPool() {
        for (auto& t : m_pool) {
            t.join();
        }
    }

private:
    std::vector<std::thread> m_pool;
};

#endif  // THREADPOOL_H
