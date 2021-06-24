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
            return std::unique_lock(m_lock_mutex);
        }

        inline std::lock_guard<std::mutex> getLockGuard()
        {
            return std::lock_guard<std::mutex>(m_lock_mutex);
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