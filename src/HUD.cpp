#include "HUD.hpp"
#include "ModelLoader.hpp"
#include <iostream>

namespace mode7
{

HUD::HUD() :
    m_healthBar(nullptr),
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
    scene = ModelLoader::openShared("assets/models/hud/health_bar.dae");
    std::cout << scene->query() << std::endl;

    m_healthBar = scene->getMesh(0);
    m_healthBar->position = glm::vec3(0, 0.8f, 0);
    //m_healthBar->rotate(0.f, M_PI / 2.f, 0.f);
    m_healthBar->scale = glm::vec3(0.025f);

    float s = 0.3f;
    //m_healthBar.scale = glm::vec3(s, s, s);
}

void HUD::update()
{
    if (!m_healthBar) return;
    m_healthBar->update();
}

void HUD::draw()
{
    if (!m_healthBar) return;
    //glEnable(GL_DEPTH_TEST);
    m_healthBar->draw(m_shader);
}

}
