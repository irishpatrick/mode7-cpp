#include "Car.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "TexCache.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Car::Car() :
    Mesh(),
    traction(0.01f),
    state(IDLE),
    tracked(false),
    speed(0.0f),
    drift(0.0f),
    throttle(0.0f),
    topSpeed(1.7f)
{

}

Car::~Car()
{

}

void Car::open(const std::string& fn)
{
    shadow.create();
    shadow.position.y = -1.f;
    sprite.addChild(&shadow);

    anim.open("assets/animations/test_anim.json");
    //material.addMap(TexCache::open("assets/textures/car.png", TexType::DIFFUSE));
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
    float delta;
    if (state == ACCEL)
    {
        delta = THROTTLE_RATE;
    }
    else if (state == BRAKE)
    {
        delta = BRAKE_RATE;
    }
    else if (state == IDLE)
    {
        delta = COAST_RATE;
    }

    throttle += delta;
    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 10.0f) throttle = 10.0f;

    state = IDLE;

    int piece = (int)fminf(9, floorf(throttle));
    float temp = velCurve[piece].solve(throttle);
    speed = topSpeed * temp / 10.0f;

    Object::position -= speed * Object::front;
    Object::position += drift * Object::right;

    drift = 0.0f;

    sprite.position = Object::position;
    sprite.scale = Object::scale;

    sprite.rx = Camera::getObject().getWorldRx();
    if (tracked)
    {
        sprite.ry = ry;
    }
    else
    {
        sprite.ry = Camera::getObject().getWorldRy();

        glm::vec3 cf = Camera::getObject().getFront();
        cf.y = 0.f;
        cf = glm::normalize(cf);
        float cross = glm::cross(front, cf).y;
        float dot = glm::dot(front, cf);

        bool north = dot > 0.5f;
        bool south = dot < -0.5f;
        bool east = cross > 0.5f;
        bool west = cross < -0.5f;

        if (north)
        {
            anim.setCurrentFrame(0);
        }
        else if (south)
        {
            anim.setCurrentFrame(3);
        }
        else if (east)
        {
            anim.setCurrentFrame(2);
        }
        else if (west)
        {
            anim.setCurrentFrame(1);
        }
    }
    

    shadow.update();
    Object::update();
    sprite.update();
}

void Car::draw(Shader& s)
{
    shadow.draw(*alt);

    s.use();
    s.setMaterial(anim);
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
    rotate(0, TURN_RATE * fminf(1.0f, (speed / (topSpeed * 0.1f))), 0);
    drift = traction * speed * fminf(1.0f, (speed / (topSpeed * 0.1f)));
}

void Car::turnRight()
{
    rotate(0, -TURN_RATE * fminf(1.0f, (speed / (topSpeed * 0.1f))), 0);
    drift = -traction * speed * fminf(1.0f, (speed / (topSpeed * 0.1f)));
}

void Car::setTracked(bool val)
{
    tracked = val;
    shadow.setTracked(tracked);
}