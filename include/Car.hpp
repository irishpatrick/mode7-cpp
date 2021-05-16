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
#include "ResponseCurve.hpp"
#include "ControlMap.hpp"
#include "ControlSlider.hpp"
#include <string>
#include <vector>

#define IDLE  0
#define ACCEL 1
#define BRAKE 2
#define STUN  3
#define LEFT  4
#define RIGHT 5
/*#define THROTTLE_RATE 0.4f
#define DRIFT_RATE 3.0f
//#define DRIFT_RATE 0.2f
//#define MAX_DRIFT 0.125f
#define MAX_DRIFT 0.6f
#define MIN_DRIFT 0.005f
//#define MIN_DRIFT 0.05f
#define COAST_RATE -0.02f
#define BRAKE_RATE 0.08f
#define TURN_RATE 0.022f
#define WHEEL_RATE 9.f*/

namespace mode7
{

typedef struct _car_properties
{
    float THROTTLE_RATE;
    float DRIFT_NORM_RATE;
    float DRIFT_LOSS_RATE;
    float DRIFT_NORM_RET;
    float DRIFT_LOSS_RET;
    float COAST_RATE;
    float BRAKE_RATE;
    float TURN_RATE;
    float WHEEL_RATE;
    float DRIFT_BASE;
    float DRIFT_NORM;
    float DRIFT_LOSS;
    float MAX_SPEED;
} car_properties;

class Car : public Mesh
{
public:

    Car();
    virtual ~Car();

    virtual void open(const std::string&);
    void openMaps(const std::string&, const std::string&, const std::string&, const std::string&);
    void parseConstants(const std::string&);
    void updateSprite();
    void updateDebugText();
    virtual void update();
    virtual void draw(Shader&);

    void updateControls();

    inline void setRacingLine(RacingLine* rl)
    {
        m_racingLine = rl;
    }

    void input();
    void stun();

    void setTracked(bool);

    float traction;

protected:

    DropShadow shadow;

    int state;
    int m_wheelState;
    int ticks;
    bool m_inStun;

    bool tracked;

    float speed;
    float drift;

    float throttle;
    float topSpeed;

    float m_power;
    float m_maxPower;
    float m_gasPos;
    float m_brakePos;
    float m_wheelPos;
    float m_driftPos;
    float m_brake;

    int m_currentZone;

    bool m_change;

    RacingLine* m_racingLine;

    Object sprite;
    Animation anim;
    DebugText m_debugText;
    std::vector<Line> velCurve;
    ResponseCurve m_vCurve;
    ResponseCurve m_wheelCurve;
    ResponseCurve m_tractionCurve;
    car_properties m_props;
    
    // controls
    ControlMap m_accelMap;
    ControlMap m_brakeMap;
    ControlMap m_driftMap;
    ControlMap m_turnMap;
    ControlSlider thr;
    ControlSlider brake;
    ControlSlider wheel;
};

}

#endif /* CAR_HPP */
