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

#ifndef HUD_HPP
#define HUD_HPP

#include "Car.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Scene.hpp"

#include <memory>

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
    std::shared_ptr<Scene> scene;
};

}

#endif /* HUD_HPP */
