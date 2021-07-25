#ifndef CAR_HPP
#define CAR_HPP

#include "gl.hpp"
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
#include "ParticleEmitter.hpp"

#include <string>
#include <vector>
#include <cstdint>

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
    class Track;

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

        void init();
        virtual void open(const std::string&);
        void openMaps(const std::string&, const std::string&, const std::string&, const std::string&);
        void parseConstants(const std::string&);

        virtual void update();
        virtual void draw(Shader&);

        void updateSprite();
        void updateControls();
        void updateTrackInfo();
        void updateEffects();

        inline void setTrack(Track* t)
        {
            m_track = t;
        }

        void input();
        void stun();

        void setTracked(bool);

        float traction;

        inline void drawEffects(bool val)
        {
            m_drawEffects = val;
        }

    #ifdef _BUILD_DEBUG_TOOLS
        void updateDebugText();
    #endif /* _BUILD_DEBUG_TOOLS */

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

        /*float m_power;
        float m_maxPower;
        float m_gasPos;
        float m_brakePos;
        float m_wheelPos;
        float m_driftPos;
        float m_brake;*/

        // track data
        Track* m_track;
        int m_currentZone;
        uint8_t m_zoneType;
        uint8_t m_zoneChangeDetected;

        bool m_change;
        bool m_drawEffects;

        Object sprite;
        Animation anim;
        car_properties m_props;
        
        // controls
        ControlMap m_accelMap;
        ControlMap m_brakeMap;
        ControlMap m_driftMap;
        ControlMap m_turnMap;
        ControlSlider m_thr;
        ControlSlider m_brake;
        ControlSlider m_wheel;
        glm::vec3 m_velMod;
        float m_turnMod;

        // particles
        ParticleEmitter m_wheelParticlesL;
        ParticleEmitter m_wheelParticlesR;

    #ifdef _BUILD_DEBUG_TOOLS
        DebugText m_debugText;
    #endif /* _BUILD_DEBUG_TOOLS */

        friend class Track;
    };
}

#endif /* CAR_HPP */
