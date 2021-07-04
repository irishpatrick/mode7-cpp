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
            m_syncdata = sd;
        }

        inline void go()
        {
            m_should_begin = true;
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

        bool m_isdone;
        bool m_running;
        bool m_autostop;
        bool m_should_begin;

        uint32_t m_id;

        std::unique_ptr<std::thread> m_thread;
        SyncData* m_syncdata;
        std::mutex m_jobq_mutex;
        std::queue<void*> m_jobq;
    };
}

#endif /* WORKER_HPP */