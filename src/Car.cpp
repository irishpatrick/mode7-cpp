#include "Car.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "TexCache.hpp"
#include "Keyboard.hpp"
#include "Util.hpp"
#include "Track.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <cassert>

using json = nlohmann::json;

namespace mode7
{
    Car::Car() :
        Mesh(),
        m_track(nullptr),
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
        topSpeed(2.0f),
        m_change(false),
        m_drawEffects(false)
    {

    }

    Car::~Car()
    {

    }

    void Car::init()
    {
        // setup particle emitters
        Texture* smoke_tex = TexCache::open("assets/textures/smoke.png", TexType::DIFFUSE);
        m_wheelParticlesL.init(2000, smoke_tex->getId());
        m_wheelParticlesR.init(2000, smoke_tex->getId());
        m_wheelParticlesL.setLifeSpread(0.5);
        m_wheelParticlesR.setLifeSpread(0.5);
        //m_wheelParticlesL.enable();
        //m_wheelParticlesR.enable();
    }

    void Car::open(const std::string& fn)
    {
        std::stringstream ss;
        ss << fn << "/";
        m_vCurve.open(ss.str() + "gas.txt");
        m_wheelCurve.open(ss.str() + "wheel.txt");
        m_tractionCurve.open(ss.str() + "drift.txt");

        shadow.create();
        shadow.position.y = -1.f;

        anim.open("assets/animations/test_anim.json");
        //material.addMap(
        //    TexCache::open("assets/textures/car.png", TexType::DIFFUSE));
        Mesh::createFromShape(Mesh::PLANE);

    #ifdef _BUILD_DEBUG_TOOLS
        m_debugText.init();
    #endif /* BUILD_DEBUG_TOOLS */
    }

    void Car::openMaps(
        const std::string& amap_fn, 
        const std::string& bmap_fn, 
        const std::string& dmap_fn, 
        const std::string& tmap_fn)
    {
        m_accelMap.open(amap_fn);
        m_brakeMap.open(bmap_fn);
        m_driftMap.open(dmap_fn);
        m_turnMap.open(tmap_fn);
    }

    void Car::parseConstants(const std::string& fn)
    {
        std::ifstream in(fn);
        if (!in)
        {
            return;
        }

        // parse file
        json obj;
        in >> obj;

        if (obj["thr_rate"].size() != 2)
        {
            std::cout << "bad config" << std::endl;
            return;
        }
        thr.setAutoUp(true);
        thr.setDownRate(obj["thr_rate"][0]);
        thr.setUpRate(obj["thr_rate"][1]);

        if (obj["brake_rate"].size() != 2)
        {
            std::cout << "bad config" << std::endl;
            return;
        }
        brake.setAutoUp(true);
        brake.setDownRate(obj["brake_rate"][0]);
        brake.setUpRate(obj["brake_rate"][1]);

        if (obj["wheel_rate"].size() != 3)
        {
            std::cout << "bad config" << std::endl;
            return;
        }
        wheel.setRestoreRates(
            obj["wheel_rate"][0], 
            obj["wheel_rate"][1], 
            obj["wheel_rate"][2]);

        m_props.MAX_SPEED = obj["max_speed"];
    }

    void Car::updateControls()
    {
        float vel_percent;
        float turn_amt = 0.5f;
        float drift_amt;
        float brake_amt;
        float top_speed = m_props.MAX_SPEED;

        thr.update();
        brake.update();
        wheel.update();

        float wp_adj = wheel.getPosition() * 0.5 + 0.5;

        vel_percent = m_accelMap.calculate(1.0, thr.getPosition());
        turn_amt = m_turnMap.calculate(wp_adj, velocity.z / top_speed);
        drift_amt = m_driftMap.calculate(fabs(wheel.getPosition()), velocity.z / top_speed);
        brake_amt = m_brakeMap.calculate(brake.getPosition(), velocity.z / top_speed);

        //std::cout << wheel.getPosition() << std::endl;

        turn_amt *= (1.0 - (brake_amt / 1.25));
        brake_amt *= (1.0 - (turn_amt / 1.25));

        float desired = vel_percent * top_speed;
        float last_vz = velocity.z;
        float last_vx = velocity.x;

        if (desired < velocity.z && thr.getSp() < 1)
        {
            thr.pushCurrentState();
            thr.setDownRate(0.07);
        }
        else if (desired >= velocity.z && thr.getSp() > 0)
        {
            thr.popCurrentState();
        }

        velocity.z = fmaxf(last_vz, desired);
        velocity.z -= brake_amt * 0.075;
        velocity.z -= 0.04 * last_vz / top_speed;
        velocity.z = fmaxf(velocity.z, 0.f);

        float drift_dir = 1.0 * (wp_adj < 0.50) - 1.0 * (wp_adj > 0.50);
        float propvx = drift_dir * drift_amt * 0.8;
        if (fabsf(propvx) < fabsf(last_vx))
        {
            velocity.x = last_vx;
        }
        else
        {
            velocity.x = propvx;
        }
        float drift_decay = 0.01;
        velocity.x -= copysignf(1.f, velocity.x) * drift_decay;
        if (fabs(velocity.x) < 1e-2)
        {
            velocity.x = 0.0;
        }
        
        float turn_rate = wheel.getPosition() * turn_amt * 0.0375;
        rotate(0, -turn_rate, 0);
    }

    void Car::update()
    {
        if (tracked)
        {
            Camera::setFOV(70 + 5 * (velocity.z / topSpeed));
        }

        updateControls();

        /*int cur = m_racingLine->getCurrentIndex(glm::vec2(position.x, position.z), m_currentZone);
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
        }*/

        Object::move();

        updateSprite();
        shadow.update();
        Object::update();
        sprite.update();

        updateTrackInfo();

        updateEffects();
    }

    void Car::updateTrackInfo()
    {
        if (!m_track)
        {
            return;
        }

        glm::vec2 pos(position.x, position.z);
        auto zones = m_track->getNearbyZones(m_currentZone);
        std::cout << "zone: " << m_currentZone << std::endl;
        assert(zones.size() > 0);
        int i = 0;
        for (auto& e : zones)
        {
            //std::cout << i++;
            if (e.second->onTrack(pos))
            {
                m_currentZone = e.first;
                //std::cout << "zone: " << m_currentZone << std::endl;
                //std::cout << "ontrack" << std::endl;
                break;
            }
            else if (e.second->onRunoff(pos))
            {
                m_currentZone = e.first;
                //std::cout << "zone: " << m_currentZone << std::endl;
                //std::cout << "onrunoff" << std::endl;
                break;
            }
            else if (e.second->onWall(pos))
            {
                m_currentZone = e.first;
                //std::cout << "zone: " << m_currentZone << std::endl;
                //std::cout << "onwall" << std::endl;
                break;
            }
            else
            {
                //std::cout << "FATAL: ZONE " << m_currentZone << std::endl;  
                //std::cout << "none" << std::endl;
            }
        }
    }

    void Car::updateEffects()
    {
        m_wheelParticlesL.position = position - (0.8f * Object::right) - Object::up;
        m_wheelParticlesR.position = position + (0.8f * Object::right) - Object::up;
        m_wheelParticlesL.setSpeed(0.0);
        m_wheelParticlesR.setSpeed(0.0);
        m_wheelParticlesR.direction = 8.0f * Object::front + Object::up;
        m_wheelParticlesL.direction = 8.0f * Object::front + Object::up;
        m_wheelParticlesL.update();
        m_wheelParticlesR.update();
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
        s.use();
        s.setMaterial(anim);
        s.setModel(sprite);
        Mesh::drawTriangles();

        //m_debugText.draw();

        if (m_drawEffects)
        {
            m_wheelParticlesL.draw();
            m_wheelParticlesR.draw();
            shadow.draw(*alt);
        }
    }

    void Car::input()
    {
        int left = Keyboard::isDown("left") || Keyboard::isDown("j");
        int right = Keyboard::isDown("right") || Keyboard::isDown("l");

        if (Keyboard::isDown("c"))
        {
            thr.down();
        }
        else if (Keyboard::isDown("x"))
        {
            brake.down();
        }
        else
        {
        }

        if (left)
        {
            wheel.left();
        }
        if (right)
        {
            wheel.right();
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

    #ifdef _BUILD_DEBUG_TOOLS
    void Car::updateDebugText()
    {
        /*if (!m_racingLine)
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
        m_debugText.setText(ss.str());*/
    }
    #endif /* _BUILD_DEBUG_TOOLS */
}
