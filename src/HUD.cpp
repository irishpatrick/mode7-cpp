/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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
