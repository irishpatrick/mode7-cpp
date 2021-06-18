#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "Worker.hpp"

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
};

}

#include "Scheduler.tpp"

#endif /* SCHEDULER_HPP */