#include "Timer.hpp"


Timer::Timer() :
    position(0),
    interval(0),
    loop(false),
    running(false)
{

}

Timer::~Timer()
{

}

void Timer::create(int inter, bool l)
{
    interval = inter;
    loop = l;
}

void Timer::start()
{
    running = true;
}

void Timer::reset()
{
    position = 0;
}

bool Timer::tick()
{
    if (!running)
    {
        return false;
    }

    ++position;

    if (position == interval)
    {
        if (loop)
        {
            reset();
        }
        else
        {
            running = false;
        }

        return true;
    }

    return false;
}
