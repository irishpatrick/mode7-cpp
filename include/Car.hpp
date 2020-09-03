#ifndef CAR_HPP
#define CAR_HPP

#include "gl.h"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Line.hpp"
#include "DropShadow.hpp"
#include "Animation.hpp"
#include "DebugText.hpp"
#include "RacingLine.hpp"
#include <string>
#include <vector>

#define IDLE  0
#define ACCEL 1
#define BRAKE 2
#define STUN  3
#define THROTTLE_RATE 0.1f
#define COAST_RATE -0.02f
#define BRAKE_RATE 0.08f
#define TURN_RATE 0.022f

namespace mode7
{

class Car : public Mesh
{
public:

    Car();
    virtual ~Car();

    virtual void open(const std::string&);
    void updateSprite();
    void updateDebugText();
    virtual void update();
    virtual void draw(Shader&);

    void updateControls();

    inline void setRacingLine(RacingLine* rl)
    {
        m_racingLine = rl;
    }

    void gas();
    void brake();
    void turnLeft();
    void turnRight();
    void stun();

    void setTracked(bool);

    float traction;

protected:

    float speed;

protected:

    DropShadow shadow;

    int state;
    int ticks;
    bool m_inStun;

    bool tracked;

    float drift;
    float throttle;
    float topSpeed;

    float m_power;
    float m_maxPower;
    float m_gasPos;
    float m_brakePos;
    float m_brake;

    int m_currentZone;

    RacingLine* m_racingLine;

    Object sprite;
    Animation anim;
    DebugText m_debugText;
    std::vector<Line> velCurve;
};

}

#endif /* CAR_HPP */
