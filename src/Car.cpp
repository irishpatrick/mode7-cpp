#include "Car.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Car::Car() :
    Mesh(),
    state(IDLE),
    tracked(false),
    speed(0.0f),
    throttlePos(0)
{

}

Car::~Car()
{

}

void Car::open(const std::string& fn)
{
    material.setDiffuseTexture(Texture::open("assets/textures/car.png"));
    Mesh::createFromShape(Mesh::PLANE);

    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open file " << fn << std::endl;
        return;
    }

    json o;
    o << in;
    in.close();

    // parse velocity curve;
    auto& vc = o["velocityCurve"];
    for (int i = 0; i < vc["numPoints"].get<int>() - 1; ++i)
    {
        int a = vc["data"][i].get<int>();
        int b = vc["data"][i + 1].get<int>();
        Line l;
        l.create(i, a, i + 1, b);
        velCurve.push_back(l);
    }
}

void Car::update()
{
    std::cout << "\rthrottle: " << throttlePos;
    std::fflush(stdout);
    if (state == ACCEL)
    {
        throttlePos += 10;
        if (throttlePos >= 10000)
        {
            throttlePos = 10000;
        }
    }
    else if (state == BRAKE)
    {
        throttlePos -= 30;
        if (throttlePos <= 0)
        {
            throttlePos = 0;
        }
    }
    else if (state == IDLE)
    {
        throttlePos -= 2;
        if (throttlePos <= 0)
        {
            throttlePos = 0;
        }
    }

    state = IDLE;

    float temp = velCurve[throttlePos / 1000].solve((float)throttlePos / 1000.0f);
    Object::position -= (0.07f * temp) * Object::front;
    Object::update();

    sprite.position = Object::position;
    sprite.scale = Object::scale;
    sprite.update();
    if (!tracked)
    {
        sprite.rotation = Camera::getObject().getWorldRotation();
    }
}

void Car::draw(Shader& s)
{
    glBindTexture(GL_TEXTURE_2D, material.getDiffuseTexture());
    s.use();
    s.setMaterial(material);
    s.setModel(sprite);

    Mesh::drawTriangles();
}

void Car::gas()
{
    state = ACCEL;
}

void Car::brake()
{
    state = BRAKE;
}

void Car::turnLeft()
{
    rotation.y += 0.03f;
}

void Car::turnRight()
{
    rotation.y -= 0.03f;
}

void Car::setTracked(bool val)
{
    tracked = val;
}