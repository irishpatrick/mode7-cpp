#include "HUD.hpp"

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
    m_healthBar.createFromShape(Mesh::PLANE);
}

void HUD::update()
{

}

void HUD::draw()
{

}

}
