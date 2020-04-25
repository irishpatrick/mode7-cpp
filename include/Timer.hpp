#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
public:
    Timer();
    ~Timer();

    void create(int, bool);
    void start();
    void reset();
    bool tick();

private:

    int position;
    int interval;
    bool loop;
    bool running;
};

#endif /* TIMER_HPP */
