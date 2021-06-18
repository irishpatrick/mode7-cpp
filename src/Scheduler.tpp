#include "Scheduler.hpp"

namespace mode7
{

template <class T>
Scheduler<T>::Scheduler()
{

}

template <class T>
Scheduler<T>::~Scheduler()
{

}

template <class T>
int Scheduler<T>::startWorkers(uint32_t n_workers)
{
    std::unique_ptr<T> created;
    for (uint32_t i = 0; i < n_workers; ++i)
    {
        created = std::make_unique<T>();
        m_workers.push_back(std::move(created));
        m_workers.back()->start(false);
    }

    return 0;
}

template <class T>
void Scheduler<T>::addJobData(void* data)
{
    m_jobdata.push_back(data);
}

template <class T>
void Scheduler<T>::distribute()
{
    int ctr = 0;
    while (ctr < m_jobdata.size())
    {
        for (int i = 0; i < m_workers.size(); ++i)
        {
            m_workers[i]->queue(m_jobdata[ctr]);
            ++ctr;
            if (ctr >= m_jobdata.size())
            {
                break;
            }
        }
    }
}

template <class T>
void Scheduler<T>::join()
{
    for (int i = 0; i < m_workers.size(); ++i)
    {
        m_workers[i]->join();
    }
}

template <class T>
void Scheduler<T>::spinWait()
{
    bool cont = false;
    while (!cont)
    {
        cont = true;
        for (int i = 0; i < m_workers.size(); ++i)
        {
            cont &= m_workers[i]->isDone();
        }
    }
}

}