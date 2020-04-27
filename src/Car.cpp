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
    drift(0.0f),
    throttle(0.0f),
    topSpeed(1.3f),
    traction(0.3f)
{

}

Car::~Car()
{

}

void Car::open(const std::string& fn)
{
    shadow.material.setDiffuseTexture(Texture::open("assets/textures/drop_shadow.png"));
    shadow.createFromShape(Mesh::PLANE);
    shadow.rotation.x = M_PI / 2.0f;
    shadow.position.y = -0.99f;
    Object::addChild(shadow);

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
    //mastd::cout << "\rthrottle: " << throttle << "           ";
    std::fflush(stdout);
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
    
    sprite.rotation = Camera::getObject().getWorldRotation();

    if (tracked)
    {
        sprite.rotation.y = rotation.y;
    }

    shadow.update();
    Object::update();
    sprite.update();
}

void Car::draw(Shader& s)
{
    shadow.draw(s);

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
    rotation.y += TURN_RATE * fminf(1.0f, (speed / (topSpeed * 0.1f)));
    drift = traction * fminf(1.0f, (speed / (topSpeed * 0.1f)));
}

void Car::turnRight()
{
    rotation.y -= TURN_RATE * fminf(1.0f, (speed / (topSpeed * 0.1f)));
    drift = -traction * fminf(1.0f, (speed / (topSpeed * 0.1f)));
}

void Car::setTracked(bool val)
{
    tracked = val;
}