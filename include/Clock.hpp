#ifndef CLOCK_HPP
#define CLOCK_HPP

namespace mode7
{

class Clock
{
public:
    
    static void start();
    static void tick();
    static void lagTick();
    static long delta();
    static bool lagging();
    static float extrapolate();
    static float fps();
};

}

#endif /* CLOCK_HPP */
