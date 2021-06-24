#include "Worker.hpp"

#include <iostream>

namespace mode7
{

Worker::Worker(uint32_t id=0) :
    m_isdone(false),
    m_should_begin(false),
    m_id(id)
{

}

Worker::~Worker()
{

}

void Worker::jobLoop(Worker* w)
{
    bool started = false;
    void* cur;
    while (w->m_running)
    {
        auto lk = w->m_syncdata->getUniqueLock();
        w->m_syncdata->getWakeWorkerCV()->wait(lk, [w]{return w->m_should_begin;});
        lk.unlock();
        w->m_syncdata->getWakeWorkerCV()->notify_one();
        if (!w->m_running)
        {
            break;
        }

        while (w->m_jobq.size() > 0)
        {
            started = true;

            //w->m_jobq_mutex.lock();
            cur = w->m_jobq.front();
            w->m_jobq.pop();
            //w->m_jobq_mutex.unlock();

            w->job(cur);
        }

        w->m_should_begin = false;

        {
            auto lg = w->m_syncdata->getLockGuard();
            w->m_syncdata->decrement();
        }
        w->m_syncdata->getWakeSchedulerCV()->notify_one();
        
        if (started && w->m_autostop)
        {
            w->m_running = false;
        }
    }
}

void Worker::queue(void* data)
{
    //m_jobq_mutex.lock();
    m_jobq.push(data);
    //m_jobq_mutex.unlock();
}

void Worker::start(bool autostop)
{
    m_running = true;
    m_autostop = autostop;
    m_thread = std::make_unique<std::thread>(jobLoop, this);
}

void Worker::join()
{
    m_thread->join();
}

size_t Worker::getQueueLen()
{
    return m_jobq.size();
}

bool Worker::isDone()
{
    return m_jobq.size() == 0;
}

}
