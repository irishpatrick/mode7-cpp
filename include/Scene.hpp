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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include "Mesh.hpp"
#include "Shader.hpp"
#include <string>
#include <sstream>

namespace mode7
{

class Scene : public Object
{
public:

    Scene();
    ~Scene();

    void addMesh(std::shared_ptr<Mesh>);
    Mesh* getMesh(unsigned int);

    void draw(Shader&);

    inline std::string query()
    {
        std::stringstream ss;
        for (unsigned int i = 0; i < meshes.size(); ++i)
        {
            ss << i << ": " << "..." << "\n";
        }

        return ss.str();
    }

private:

    std::vector<std::shared_ptr<Mesh>> meshes;
};

}

#endif /* SCENE_HPP */
