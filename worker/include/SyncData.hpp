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

#ifndef SYNCDATA_HPP
#define SYNCDATA_HPP

#include <mutex>
#include <condition_variable>
#include <memory>
#include <cstdint>

namespace mode7
{
    class SyncData
    {
    public:
        SyncData() :
            m_numworkers(0)
        {
            resetCounter();
        }
        
        ~SyncData()
        {

        }

        inline void setNumWorkers(uint32_t num)
        {
            m_numworkers = num;
            resetCounter();
        }

        inline std::condition_variable* getWakeWorkerCV()
        {
            return &m_wake_worker;
        }

        inline std::condition_variable* getWakeSchedulerCV()
        {
            return &m_wake_scheduler;
        }

        inline std::unique_lock<std::mutex> getUniqueLock()
        {
            return (std::unique_lock<std::mutex>(m_lock_mutex));
        }

        inline std::mutex& getLockMutex()
        {
            return m_lock_mutex;
        }

        inline void resetCounter()
        {
            m_counter = m_numworkers;
        }

        inline void decrement()
        {
            --m_counter;
        }

        inline uint32_t getCounter()
        {
            return m_counter;
        }

    private:
        std::condition_variable m_wake_worker;        
        std::condition_variable m_wake_scheduler;
        std::mutex m_lock_mutex;
        uint32_t m_counter;
        uint32_t m_numworkers;
    };
}

#endif /* SYNCDATA_HPP */