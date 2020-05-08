#ifndef CAR_HPP
#define CAR_HPP

#include "gl.h"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Line.hpp"
#include "DropShadow.hpp"
#include <string>
#include <vector>

#define IDLE 0
#define ACCEL 1
#define BRAKE 2
#define THROTTLE_RATE 0.1f
#define COAST_RATE -0.02f
#define BRAKE_RATE -0.12f
#define TURN_RATE 0.022f

class Car : public Mesh
{
public:

    Car();
    virtual ~Car();

    virtual void open(const std::string&);
    virtual void update();
    virtual void draw(Shader&);

    void gas();
    void brake();
    void turnLeft();
    void turnRight();

    void setTracked(bool);

    float traction;

private:

    DropShadow shadow;

    int state;
    
    bool tracked;
    float speed;
    float drift;
    float throttle;
    float topSpeed;
    
    Object sprite;
    GLuint textures[4];
    std::vector<Line> velCurve;
};

#endif /* CAR_HPP */