#ifndef HUD_HPP
#define HUD_HPP

#include "Car.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

namespace mode7
{

class HUD
{
public:

    HUD();
    ~HUD();

    void init();
    void update();
    void draw();

    inline void setCar(Car* c)
    {
        m_car = c;
    }

private:

    Car* m_car;

    Mesh m_health;
    Mesh* m_healthBar;
    Mesh m_gasGauge;
    Mesh m_gas;
    Mesh m_brakeGauge;
    Mesh m_brake;
    Shader m_shader;
    Scene scene;
};

}

#endif /* HUD_HPP */
