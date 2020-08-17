#ifndef CAR_HPP
#define CAR_HPP

#include "gl.h"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Line.hpp"
#include "DropShadow.hpp"
#include "Animation.hpp"
#include <string>
#include <vector>

#define IDLE  0
#define ACCEL 1
#define BRAKE 2
#define STUN  3
#define THROTTLE_RATE 0.1f
#define COAST_RATE -0.02f
#define BRAKE_RATE 0.12f
#define TURN_RATE 0.022f

class Car : public Mesh
{
public:

    Car();
    virtual ~Car();

    virtual void open(const std::string&);
    void updateSprite();
    virtual void update();
    virtual void draw(Shader&);

    void updateControls();

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

    Object sprite;
    Animation anim;
    std::vector<Line> velCurve;
};

#endif /* CAR_HPP */
