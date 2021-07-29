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

#ifndef SHADER_HPP
#define SHADER_HPP

#include "gl.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include <string>
#include <vector>

#define MAT(x) &(x[0][0])
#define VEC(x) &(x[0])

namespace mode7
{

class Shader
{
public:

    Shader();
    ~Shader();

    int open(const std::string&, const std::string&);
    void cacheCameraMatrices();
    void cacheLocations();

    void use();
    void onlyUse();
    void setMaterial(Material&);
    void setModel(Object&);

    inline GLuint pid()
    {
        return id;
    }

private:

    GLuint id;
    GLuint projection;
    GLuint view;
    GLuint model;
    GLuint diffuseTexture;
    GLuint uv_tile;

    std::vector<GLuint> m_diffuseMaps;
    std::vector<GLuint> m_specularMaps;
};

}

#endif /* SHADER_HPP */