#ifndef AI_HPP
#define AI_HPP

#include "Car.hpp"
#include "RacingLine.hpp"
#include <cstdint>

class AI : public Car
{
public:

    AI();
    virtual ~AI();

    void setRacingLine(RacingLine);

    virtual void update();

private:

    int ticksLeft;
    RacingLine line;
};

#endif /* AI_HPP */