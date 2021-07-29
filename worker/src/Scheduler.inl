/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/



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
            m_syncdata.setNumWorkers(n_workers);
            created = std::make_unique<T>(i + 1);
            created->setSyncData(&m_syncdata);
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
        uint32_t ctr = 0;
        while (ctr < m_jobdata.size())
        {
            for (uint32_t i = 0; i < m_workers.size(); ++i)
            {
                m_workers[i]->queue(m_jobdata[ctr]);
                ++ctr;
                if (ctr >= m_jobdata.size())
                {
                    break;
                }
            }
        }

        // local block for destruction of lock_guard
        {
            std::lock_guard<std::mutex> lg(m_syncdata.getLockMutex());
            for (auto& e : m_workers)
            {
                e->go();
            }
        }   
        m_syncdata.getWakeWorkerCV()->notify_all();

        SyncData* sdat = &m_syncdata;

        // local block for destruction of unique_lock
        {
            auto lk = m_syncdata.getUniqueLock();
            m_syncdata.getWakeSchedulerCV()->wait(lk, [sdat]{ return sdat->getCounter() == 0; });
        }

        m_syncdata.resetCounter();
    }

    template <class T>
    void Scheduler<T>::shutdown()
    {
        // local block for destruction of lock_guard
        {
            std::lock_guard<std::mutex> lg(m_syncdata.getLockMutex());
            for (auto& e : m_workers)
            {
                e->go();
                e->stop();
            }
        }   
        m_syncdata.getWakeWorkerCV()->notify_all();

        join();
    }

    template <class T>
    void Scheduler<T>::join()
    {
        for (uint32_t i = 0; i < m_workers.size(); ++i)
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