#ifndef AI_HPP
#define AI_HPP

#include "Car.hpp"
#include "RacingLine.hpp"
#include <cstdint>

namespace mode7
{

class AI : public Car
{
public:

    AI();
    virtual ~AI();

    void control();
    virtual void update();

private:
        
    int m_actionCooldown;
    int m_currentZone;
    int m_lastAction;
};

}

#endif /* AI_HPP */
