#ifndef CLOCK_HPP
#define CLOCK_HPP

class Clock
{
public:
    
    static void start();
    static void tick();
    static void lagTick();
    static long delta();
    static bool lagging();
    static float extrapolate();
};

#endif /* CLOCK_HPP */