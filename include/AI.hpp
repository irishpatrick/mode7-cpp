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

    void setRacingLine(RacingLine*);
    void control();
    virtual void update();

private:

    int m_currentZone;
    RacingLine* m_racingLine;
};

#endif /* AI_HPP */