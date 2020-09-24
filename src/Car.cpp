#include "Car.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "TexCache.hpp"
#include "Keyboard.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mode7
{

Car::Car() :
    Mesh(),
    traction(0.005f),
    state(IDLE),
    m_wheelState(IDLE),
    ticks(0),
    m_inStun(false),
    tracked(false),
    speed(0.0f),
    drift(0.0f),
    throttle(0.0f),
    m_gasPos(0.f),
    m_brakePos(0.f),
    m_wheelPos(50.f),
    m_power(0.f),
    m_brake(0.f),
    m_maxPower(3.f),
    topSpeed(2.5f),
    m_change(false),
    m_racingLine(nullptr)
{

}

Car::~Car()
{

}

void Car::open(const std::string& fn)
{
    m_debugText.init();
    //m_vCurve.open("assets/cars/higha.txt");
    m_vCurve.open("assets/cars/vctest.txt");
    m_wheelCurve.open("assets/cars/wheel_response.txt");

    shadow.create();
    shadow.position.y = -1.f;
    //addChild(&shadow);

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
    in >> o;
    in.close();
}

void Car::updateControls()
{
    if (m_inStun)
    {
        --ticks;
        if (ticks == 0)
        {
            m_inStun = false;
            traction = 0.005;
        }
    }
    else
    {
        if (state == ACCEL)
        {
            if (m_change)
            {
                m_change = false;
                //std::cout << "start at " << velocity.z / topSpeed * 100.f << std::endl;
                m_gasPos = m_vCurve.getX(velocity.z / topSpeed * 100.f);
            }
            m_gasPos += THROTTLE_RATE;
            m_brakePos = 0.f;
        }
        else if (state == BRAKE)
        {
            m_brakePos += BRAKE_RATE;
            m_gasPos = 0.f;
        }
        else if (state == IDLE)
        {
            m_gasPos = 0.f;
            m_brakePos = 0.f;
        }

        if (m_gasPos < 0.0f)
        {
            m_gasPos = 0.0f;
        }
        else if (m_gasPos > 100.f)
        {
            m_gasPos = 100.0f;
        }
        if (m_brakePos < 0.f) m_brakePos = 0.f;
        if (m_brakePos > 10.0f) m_brakePos = 10.0f;
    }

    /*int pt = (int)fminf(9, floorf(m_gasPos));
    float output = velCurve[pt].solve(m_gasPos);
    m_power = m_maxPower * output / 10.f;
    m_brake = (m_brakePos / ((m_brakePos * m_brakePos / 10.f) + 1.f)) * 0.02f;*/

    if (m_wheelState == LEFT)
    {
        m_wheelPos -= WHEEL_RATE;
    }
    else if (m_wheelState == RIGHT)
    {
        m_wheelPos += WHEEL_RATE;
    }
    else if (m_wheelState == IDLE)
    {
        float dot = m_wheelPos - 50.f;
        if (dot < 0.f)
        {
            m_wheelPos += WHEEL_RATE;
        }
        else if (dot > 0.f)
        {
            m_wheelPos -= WHEEL_RATE;
        }

        if (fabsf(m_wheelPos - 50.f) < 0.1f)
        {
            m_wheelPos = 50.f;
        }
    }

    // bounds checking
    if (m_wheelPos <= 0.f) m_wheelPos = 0.f;
    if (m_wheelPos >= 100.f) m_wheelPos = 100.f;
    
    //std::cout << m_wheelPos << "," << m_wheelCurve.getY(m_wheelPos) << std::endl;
    
    float turn_rate = (m_wheelCurve.getY(m_wheelPos)) / 100.f * TURN_RATE;
    rotate(0, -turn_rate * fminf(1.0f, (velocity.z / (topSpeed * 0.1f))), 0);
    drift = traction * velocity.z * fminf(1.0f, (velocity.z / (topSpeed * 0.1f)));

    if (state == ACCEL)
    {
        velocity.z = (m_vCurve.getY(m_gasPos)) / 100.f * topSpeed;
        //std::cout << "gas: " <<  m_gasPos << "\t" << m_vCurve.getY(m_gasPos) << "\t" << velocity.z << std::endl;
    }
    else if (state == BRAKE)
    {
        float sign = 0.f;
        if (velocity.z < -0.01f || velocity.z > 0.01f)
        {
            sign = velocity.z / fabs(velocity.z);
        }

        velocity.z += -sign * 0.03f;
    }
    else if (state == IDLE)
    {
        float sign = 0.f;
        if (velocity.z < -0.01f || velocity.z > 0.01f)
        {
            sign = velocity.z / fabs(velocity.z);
        }

        if (fabsf(velocity.z) > 0.001f)
        {
            velocity.z += -sign * 0.003f;
        }
        else
        {
            velocity.z = 0;
        }
    }
}

void Car::updateDebugText()
{
    if (!m_racingLine)
    {
        return;
    }

    int cur = m_racingLine->getCurrentIndex(glm::vec2(position.x, position.z), m_currentZone);
    if (cur >= 0)
    {
        m_currentZone = cur;
    }

    glm::vec2 front_2d = glm::vec2(front.x, front.z);
    glm::vec2 pos_2d = glm::vec2(position.x, position.z);

    Line2D line = m_racingLine->getLine(m_currentZone);
    Line2D next = m_racingLine->getNext(m_currentZone);

    float distToLine = line.distTo(pos_2d);
    float distToNext = glm::length((next.p() - pos_2d));
    float absDistToLine = fabsf(distToLine);
    float dot_line = glm::dot(
        line.v(), 
        front_2d
    );
    glm::vec3 cross_line_vec = glm::cross(
        glm::vec3(line.v().x, 0.f, line.v().y),
        Car::front
    );
    float cross_line = cross_line_vec.y;
    float dot_next = glm::dot(
        next.v(),
        front_2d
    );
    glm::vec3 cross_next_vec = glm::cross(
        //glm::vec3(line.normal().x, 0.f, line.normal().y),
        glm::vec3(line.v().x, 0.f, line.v().y),
        Car::front
    );
    float cross_next = cross_next_vec.y;

    int moving_right = cross_line > 0.f;
    int moving_left = cross_line < 0.f;
    int right_of_line = distToLine > 0.f;
    int left_of_line = distToLine < 0.f;
    int dist_threshold = absDistToLine > 0.1f;

    Rect r = m_racingLine->getRect(m_currentZone);
    Rect rp = m_racingLine->getRect(m_racingLine->getPrevIndex(m_currentZone));
    Rect rn = m_racingLine->getRect(m_racingLine->getNextIndex(m_currentZone));
    if (!r.checkIntersect(pos_2d) && !rn.checkIntersect(pos_2d))
    {
        drift = 0.03f;
    }

    std::stringstream ss;
    ss <<
        m_currentZone << "\n" <<
        distToLine << ", " << distToNext << "\n" <<
        moving_left << ", " << moving_right << "\n" << 
        left_of_line << ", " << right_of_line << "\n" <<
        dist_threshold << "\n" << 
        cross_next << ", " << distToNext << ", " << fabsf(cross_next) / distToNext << "\n";
    m_debugText.setText(ss.str());
}

void Car::update()
{
    updateDebugText();
    updateControls();

    Object::move();

    /*float sign = 0.f;
    if (velocity.z < -0.01f || velocity.z > 0.01f)
    {
        sign = velocity.z / fabs(velocity.z);
    }*/

    //velocity.z += m_power - (velocity.z * (0.005f + m_brake));
    //velocity.x += drift - (velocity.x * 0.05f);
    //velocity.x += drift - (velocity.x * 0.005f);
    //drift = 0.0f;

    updateSprite();
    shadow.update();
    Object::update();
    sprite.update();
}

void Car::updateSprite()
{
    sprite.position = Object::position;
    sprite.scale = Object::scale;
    shadow.position = Object::position - glm::vec3(0.f, 1.f, 0.f);
    //shadow.scale = Object::scale;

    shadow.ry = Camera::getObject().getWorldRy();
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
}

void Car::draw(Shader& s)
{
    shadow.draw(*alt);

    s.use();
    s.setMaterial(anim);
    s.setModel(sprite);
    Mesh::drawTriangles();

    m_debugText.draw();
}

void Car::input()
{

    int left = Keyboard::isDown("left");
    int right = Keyboard::isDown("right");

    if (Keyboard::isDown("c"))
    {
        if (state != ACCEL)
        {
            m_change = true;
        }
        state = ACCEL;
    }
    else if (Keyboard::isDown("x"))
    {
        state = BRAKE;
    }
    else
    {
        state = IDLE;
    }

    if (left)
    {
        m_wheelState = LEFT;        
        //turnLeft();
    }
    if (right)
    {
        m_wheelState = RIGHT;
        //turnRight();
    }
    if (!left && !right)
    {
        m_wheelState = IDLE;
    }
}

void Car::gas()
{
}

void Car::brake()
{
}

void Car::turnLeft()
{
    rotate(0, TURN_RATE * fminf(1.0f, (velocity.z / (topSpeed * 0.1f))), 0);
    drift = traction * velocity.z * fminf(1.0f, (velocity.z / (topSpeed * 0.1f)));
}

void Car::turnRight()
{
    rotate(0, -TURN_RATE * fminf(1.0f, (velocity.z / (topSpeed * 0.1f))), 0);
    drift = -traction * velocity.z * fminf(1.0f, (velocity.z / (topSpeed * 0.1f)));
}

void Car::setTracked(bool val)
{
    tracked = val;
    shadow.setTracked(tracked);
}

void Car::stun()
{
    m_gasPos = 0.f;
    m_brakePos = 0.f;
    m_inStun = true;
    traction = 0.1;
    ticks = 40;
}

}
