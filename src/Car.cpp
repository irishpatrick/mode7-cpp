#include "Car.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "TexCache.hpp"
#include "Keyboard.hpp"
#include "Util.hpp"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mode7
{

Car::Car() :
    Mesh(),
    traction(0.5f),
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
    m_driftPos(50.f),
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

void Car::parseConfig(const std::string& fn)
{
    std::cout << "load car config" << std::endl;
    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "couldn't open " << fn << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> parts;
    while (std::getline(in, line))
    {
        boost::split(parts, line, boost::is_any_of("="));
        if (parts.size() < 2)
        {
            std::cout << "bad line: " << line << std::endl;
            continue;
        }

        if (parts[0] == "THROTTLE_RATE")
        {
            m_props.THROTTLE_RATE = atof(parts[1].c_str());
        }
        else if (parts[0] == "DRIFT_BASE")
        {
            m_props.DRIFT_BASE = atof(parts[1].c_str());
        }
        else if (parts[0] == "DRIFT_NORM")
        {
            std::cout << line << std::endl;
            m_props.DRIFT_NORM = atof(parts[1].c_str()); 
        } 
        else if (parts[0] == "DRIFT_LOSS")
        {
            m_props.DRIFT_LOSS = atof(parts[1].c_str());
        }
        else if (parts[0] == "DRIFT_NORM_RET")
        {
            m_props.DRIFT_NORM_RET = atof(parts[1].c_str());
        }
        else if (parts[0] == "DRIFT_LOSS_RET")
        {
            m_props.DRIFT_LOSS_RET = atof(parts[1].c_str());
        }
        else if (parts[0] == "DRIFT_NORM_RATE")
        {
            m_props.DRIFT_NORM_RATE = atof(parts[1].c_str());
        }
        else if (parts[0] == "DRIFT_LOSS_RATE")
        {
            m_props.DRIFT_LOSS_RATE = atof(parts[1].c_str());
        }
        else if (parts[0] == "TURN_RATE")
        {
            m_props.TURN_RATE = atof(parts[1].c_str());
        }
        else if (parts[0] == "WHEEL_RATE")
        {
            m_props.WHEEL_RATE = atof(parts[1].c_str());
        }
        else if (parts[0] == "BRAKE_RATE")
        {
            m_props.BRAKE_RATE = atof(parts[1].c_str());
        }
        else if (parts[0] == "COAST_RATE")
        {
            m_props.COAST_RATE = atof(parts[1].c_str());
        }
        else
        {
            std::cout << "fatal at line " << line << std::endl;
            assert(1 == 0);
        }
        
    }

    std::cout << m_props.DRIFT_NORM << std::endl;
    assert(m_props.DRIFT_NORM > 0);
    assert(m_props.DRIFT_NORM_RATE > 0);
}

void Car::open(const std::string& fn)
{
    parseConfig("assets/cars/config.txt");
    std::stringstream ss;
    ss << fn << "/";
    m_debugText.init();
    //m_vCurve.open("assets/cars/higha.txt");
    m_vCurve.open(ss.str() + "gas.txt");
    m_wheelCurve.open(ss.str() + "wheel.txt");
    m_tractionCurve.open(ss.str() + "drift.txt");

    shadow.create();
    shadow.position.y = -1.f;
    //addChild(&shadow);

    anim.open("assets/animations/test_anim.json");
    //material.addMap(TexCache::open("assets/textures/car.png", TexType::DIFFUSE));
    Mesh::createFromShape(Mesh::PLANE);
}

void Car::updateControls()
{
    if (m_inStun)
    {
        --ticks;
        if (ticks == 0)
        {
            m_inStun = false;
            traction = 2.f;
        }
    }
    else
    {
        if (state == ACCEL)
        {
            if (m_change)
            {
                m_change = false;
                m_gasPos = m_vCurve.getX(speed / topSpeed * 100.f);
            }
            m_gasPos += m_props.THROTTLE_RATE;
            m_brakePos = 0.f;
        }
        else if (state == BRAKE)
        {
            m_brakePos += m_props.BRAKE_RATE;
            m_gasPos = 0.f;
        }
        else if (state == IDLE)
        {
            m_gasPos = 0.f;
            m_brakePos = 0.f;
        }

        m_gasPos = Util::constrain(m_gasPos, 0.f, 100.f);
        m_brakePos = Util::constrain(m_brakePos, 0.f, 10.f);
    }

    /*int pt = (int)fminf(9, floorf(m_gasPos));
    float output = velCurve[pt].solve(m_gasPos);
    m_power = m_maxPower * output / 10.f;
    m_brake = (m_brakePos / ((m_brakePos * m_brakePos / 10.f) + 1.f)) * 0.02f;*/

    if (m_wheelState == LEFT)
    {
        m_wheelPos -= m_props.WHEEL_RATE;
        m_driftPos -= m_props.DRIFT_NORM_RATE;
    }
    else if (m_wheelState == RIGHT)
    {
        m_wheelPos += m_props.WHEEL_RATE;
        m_driftPos += m_props.DRIFT_NORM_RATE;
    }
    else if (m_wheelState == IDLE)
    {
        float dot = m_wheelPos - 50.f;
        if (fabsf(m_wheelPos - 50.f) < m_props.WHEEL_RATE / 2.f)
        {
            m_wheelPos = 50.f;
        }
        else if (dot < 0.f)
        {
            m_wheelPos += m_props.WHEEL_RATE;
        }
        else if (dot > 0.f)
        {
            m_wheelPos -= m_props.WHEEL_RATE;
        }
        
        dot = m_driftPos - 50.f;
        if (fabsf(m_driftPos - 50.f) < m_props.DRIFT_NORM_RATE * m_props.DRIFT_NORM_RET / 2.f)
        {
            m_driftPos = 50.f;
        }
        else if (dot < 0.f)
        {
            m_driftPos += m_props.DRIFT_NORM_RATE * m_props.DRIFT_NORM_RET;
        }
        else if (dot > 0.f)
        {
            m_driftPos -= m_props.DRIFT_NORM_RATE * m_props.DRIFT_NORM_RET;
        }
    }

    // bounds checking
    m_wheelPos = Util::constrain(m_wheelPos, 0.f, 100.f);
    m_driftPos = Util::constrain(m_driftPos, 0.f, 100.f);

    // turning
    float turn_rate = (m_wheelCurve.getY(m_wheelPos)) / 100.f * m_props.TURN_RATE;
    rotate(0, -turn_rate * fminf(1.0f, (speed / (topSpeed * 0.1f))), 0);
    
    // drifting
    drift = -(m_tractionCurve.getY(m_driftPos)) / 100.f * (m_props.DRIFT_NORM * (speed / topSpeed) + m_props.DRIFT_BASE); 

    if (state == ACCEL)
    {
        speed = (m_vCurve.getY(m_gasPos)) / 100.f * topSpeed;
    }
    else if (state == BRAKE)
    {
        speed += copysignf(0.03f, -speed);
    }
    else if (state == IDLE)
    {
        if (fabsf(speed) > 0.001f)
        {
            speed += copysignf(0.006f, -speed);
        }
        else
        {
            speed = 0;
        }
    }

    // apply speed and drift to velocity
    velocity.x = drift;
    velocity.z = speed;
}

void Car::update()
{
    updateDebugText();
    updateControls();

    int cur = m_racingLine->getCurrentIndex(glm::vec2(position.x, position.z), m_currentZone);
    if (cur >= 0)
    {
        m_currentZone = cur;
    }
    glm::vec2 pos_2d = glm::vec2(position.x, position.z);
    Rect r = m_racingLine->getRect(m_currentZone);
    Rect rp = m_racingLine->getRect(m_racingLine->getPrevIndex(m_currentZone));
    Rect rn = m_racingLine->getRect(m_racingLine->getNextIndex(m_currentZone));
    if (!r.checkIntersect(pos_2d) && !rn.checkIntersect(pos_2d))
    {
        //drift = 0.03f;
    }

    Object::move();

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
    }
    if (right)
    {
        m_wheelState = RIGHT;
    }
    if (!left && !right)
    {
        m_wheelState = IDLE;
    }
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

}
