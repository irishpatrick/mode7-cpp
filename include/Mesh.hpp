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

#ifndef MESH_HPP
#define MESH_HPP

#include "Object.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "gl.hpp"

namespace mode7
{

class Vertex
{
public:

    Vertex();
    ~Vertex();

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh : public Object
{
public:
    
    Mesh();
    virtual ~Mesh();

    void init();
    void setMaterial(Material&);
    void createFromBuffer(const float*, GLuint);
    void createFromArrays(std::vector<Vertex>&, std::vector<unsigned int>&);
    void createFromShape(int);
    void drawTriangles();
    virtual void update();
    virtual void update(glm::mat4&);
    virtual void draw(Shader&);
    void setAltShader(Shader&);
    
    inline void destroy()
    {
        if (vao > 0)
        {
            glDeleteBuffers(1, &vao);
            vao = 0;
        }
        
        if (vbo > 0)
        {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }
        
        if (ebo > 0)
        {
            glDeleteBuffers(1, &ebo);
            ebo = 0; 
        }
    }

    Material material;
    bool visible;

    static const int PLANE = 0;

protected:

    Shader* alt;

private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    bool elements;
    int triangles;
};

}

#endif /* MESH_HPP */
