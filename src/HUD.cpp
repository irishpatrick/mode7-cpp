#include "HUD.hpp"
#include "ModelLoader.hpp"

namespace mode7
{

HUD::HUD() : 
    m_car(nullptr)
{

}

HUD::~HUD()
{

}

void HUD::init()
{
    m_shader.open("assets/shaders/hud_v.glsl", "assets/shaders/hud_f.glsl");
    m_health.createFromShape(Mesh::PLANE);
    //m_healthBar.createFromShape(Mesh::PLANE);
    m_healthBar = *(ModelLoader::open("assets/models/hud/health_bar.dae").getMesh(0));
}

void HUD::update()
{
    m_healthBar.update();
}

void HUD::draw()
{
    m_shader.setModel(m_healthBar);
    m_shader.use();
    m_healthBar.draw(m_shader);
}

}
