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

#include "Track.hpp"
#include "Tree.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace mode7
{

using json = nlohmann::json;

static Mesh mesh;
static std::vector<Tree> trees;

void Track::open(const std::string& fn)
{
    mesh.createFromShape(Mesh::PLANE);
    mesh.scale = glm::vec3(300.0f);
    //mesh.material.addDiffuseMap(Texture::open("assets/textures/track.png"));
    mesh.setRotation(M_PI / 2.0f, 0, 0);

    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return;
    }

    json o;
    in >> o;
    in.close();
}

void Track::update()
{
    mesh.update();
}

void Track::draw(Shader& s)
{
    mesh.draw(s);
    for (auto& e : trees)
    {
        e.draw(s);
    }
}

void Track::destroy()
{
}

}