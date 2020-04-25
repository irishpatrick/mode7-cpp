#ifndef CAR_HPP
#define CAR_HPP

#include "gl.h"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Line.hpp"
#include <string>
#include <vector>

#define IDLE 0
#define ACCEL 1
#define BRAKE 2

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

private:

    int state;
    
    bool tracked;
    float speed;

    Object sprite;
    GLuint textures[4];

    int throttlePos;
    std::vector<Line> velCurve;
};

#endif /* CAR_HPP */