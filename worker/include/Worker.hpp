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

#ifndef WORKER_HPP
#define WORKER_HPP

#include "SyncData.hpp"

#include <thread>
#include <mutex>
#include <memory>
#include <queue>
#include <cstdint>

namespace mode7
{
    class Worker
    {
    public:
        Worker(uint32_t);
        virtual ~Worker();

        virtual void* job(void*) = 0;

        inline void setSyncData(SyncData* sd)
        {
            m_syncData = sd;
        }

        inline void go()
        {
            m_shouldBegin = true;
        }

        inline void stop()
        {
            m_running = false;
        }

        void start(bool);
        void join();
        void queue(void*);

        size_t getQueueLen();
        bool isDone();

    private:
        static void jobLoop(Worker*);

        bool m_isDone;
        bool m_running;
        bool m_autoStop;
        bool m_shouldBegin;

        uint32_t m_id;

        std::unique_ptr<std::thread> m_thread;
        SyncData* m_syncData;
        std::mutex m_jobqMutex;
        std::queue<void*> m_jobq;
    };
}

#endif /* WORKER_HPP */