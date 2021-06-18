#ifndef WORKER_HPP
#define WORKER_HPP

#include <thread>
#include <mutex>
#include <memory>
#include <queue>

namespace mode7
{

class Worker
{
public:
    Worker();
    virtual ~Worker();

    virtual void* job(void*) = 0;

    void start(bool);
    void stop();
    void join();
    void queue(void*);

    size_t getQueueLen();
    bool isDone();

private:
    static void jobLoop(Worker*);

    bool m_isdone;
    bool m_running;
    bool m_autostop;

    std::unique_ptr<std::thread> m_thread;
    std::mutex m_jobq_mutex;
    std::queue<void*> m_jobq;
};

}

#endif /* WORKER_HPP */