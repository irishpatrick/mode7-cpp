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

#include "Worker.hpp"

#include <iostream>

namespace mode7
{
    Worker::Worker(uint32_t id=0) :
        m_isDone(false),
        m_running(false),
        m_autoStop(false),
        m_shouldBegin(false),
        m_id(id),
        m_thread(),
        m_syncData(nullptr),
        m_jobqMutex(),
        m_jobq()
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
            auto lk = w->m_syncData->getUniqueLock(); // lock here
            w->m_syncData->getWakeWorkerCV()->wait(lk, [w]{return w->m_shouldBegin;});
            lk.unlock();
            w->m_syncData->getWakeWorkerCV()->notify_one();
            if (!w->m_running)
            {
                break;
            }

            while (w->m_jobq.size() > 0)
            {
                started = true;

                cur = w->m_jobq.front();
                w->m_jobq.pop();

                w->job(cur);
            }

            w->m_shouldBegin = false;

            // separate scope enables delete after block
            {
                std::lock_guard<std::mutex> lg(w->m_syncData->getLockMutex());
                w->m_syncData->decrement();
            }
            w->m_syncData->getWakeSchedulerCV()->notify_one();
            
            if (started && w->m_autoStop)
            {
                w->m_running = false;
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
        m_autoStop = autostop;
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
