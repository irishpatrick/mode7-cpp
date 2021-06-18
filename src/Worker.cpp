#include "Worker.hpp"

namespace mode7
{

Worker::Worker()
{
 
}

Worker::~Worker()
{

}

void Worker::jobLoop()
{
    bool started = false;
    while (m_running)
    {
        while (m_jobq.size() > 0)
        {
            started = true;
            job(m_jobq.front());
            m_jobq.pop();
        }
        
        if (started && m_autostop)
        {
            m_running = false;
        }
    }
}

void Worker::queue(void* data)
{
    m_jobq.push(data);
}

void Worker::start(bool autostop)
{
    m_running = true;
    m_autostop = autostop;
}

void Worker::stop()
{
    m_running = false;
}

void Worker::join()
{
    m_thread->join();
}

size_t Worker::getQueueLen()
{
    return m_jobq.size();
}

}
