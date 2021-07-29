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

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Worker.hpp"
#include "SyncData.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

namespace mode7
{

template <class T>
class Scheduler
{
BOOST_STATIC_ASSERT((boost::is_base_of<Worker, T>::value));

public:
    Scheduler();
    ~Scheduler();

    int startWorkers(uint32_t);
    void shutdown();
    void addJobData(void*);
    void distribute();
    void join();
    void spinWait();
    
    inline void distributeAndJoin()
    {
        distribute();
        join();
    }

private:
    std::vector<std::unique_ptr<T>> m_workers;
    std::vector<void*> m_jobdata;
    SyncData m_syncdata;
};

}

#include "Scheduler.inl"

#endif /* SCHEDULER_HPP */