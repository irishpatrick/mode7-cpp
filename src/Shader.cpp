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

#include "Shader.hpp"
#include "Camera.hpp"
#include "Material.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>

namespace mode7
{

static std::string openFile(const std::string& fn)
{
    std::ifstream fp(fn);
    if (!fp)
    {
        return "";
    }

    std::stringstream code;
    code << fp.rdbuf();
    fp.close();

    return code.str();
}

static int compile(GLuint id, const std::string& code)
{
    GLint res = 0;
    int loglen;

    const char* ptr = code.c_str();
    glShaderSource(id, 1, &ptr, nullptr);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0)
    {
        std::vector<char> errMsg;
        errMsg.reserve(loglen + 1);
        glGetShaderInfoLog(id, loglen, NULL, &errMsg[0]);
        printf("%s\n", &errMsg[0]);
    }

    return res;
}

Shader::Shader() :
    id(0),
    projection(0),
    view(0),
    model(0),
    diffuseTexture(0),
    uv_tile(0)
{
}

Shader::~Shader()
{
}

int Shader::open(const std::string& vfn, const std::string& ffn)
{
    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vs_code = openFile(vfn);
    std::string fs_code = openFile(ffn);
    if (!vs_code.size() || !fs_code.size())
    {
        return -1;
    }

    std::cout << "compile " << vfn << std::endl;
    compile(vsid, vs_code);
    ec;
    std::cout << "compile " << ffn << std::endl;
    compile(fsid, fs_code);
    ec;
    
    GLuint pid = glCreateProgram();
    glAttachShader(pid, vsid);
    glAttachShader(pid, fsid);
    glLinkProgram(pid);
    ec;

    GLint res;
    int loglen;

    glGetProgramiv(pid, GL_LINK_STATUS, &res);
    glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0)
    {
        std::vector<char> errMsg;
        errMsg.reserve(loglen + 1);
        glGetShaderInfoLog(id, loglen, NULL, &errMsg[0]);
        printf("%s\n", &errMsg[0]);
    }

    glDetachShader(pid, vsid);
    glDetachShader(pid, fsid);
    glDeleteShader(vsid);
    glDeleteShader(fsid);

    id = pid;
    glUseProgram(id);

    ec;

    cacheLocations();

    return 0;
}

void Shader::cacheCameraMatrices()
{
    projection = glGetUniformLocation(id, "p");
    view = glGetUniformLocation(id, "v");
}

void Shader::cacheLocations()
{
    uint32_t n_diffuseMaps = 4;
    uint32_t n_specularMaps = 4;

    cacheCameraMatrices();
    
    model = glGetUniformLocation(id, "m");
    diffuseTexture = glGetUniformLocation(id, "tex");
    uv_tile = glGetUniformLocation(id, "uv_tile");

    ec;

    std::stringstream ss;
    m_diffuseMaps.reserve(n_diffuseMaps);
    for (uint32_t i = 0; i < n_diffuseMaps; ++i)
    {
        ss << "diffuseMap[" << i << "]";
        m_diffuseMaps.push_back(glGetUniformLocation(id, ss.str().c_str()));
        ss.clear();
    }

    m_specularMaps.reserve(n_specularMaps);
    for (uint32_t i = 0; i < n_specularMaps; ++i)
    {
        ss << "specularMap[" << i << "]";
        m_specularMaps.push_back(glGetUniformLocation(id, ss.str().c_str()));
        ss.clear();
    }
}

void Shader::onlyUse()
{
    glUseProgram(id);
}

void Shader::use()
{
    glUseProgram(id);
    //ec;

    glUniformMatrix4fv(projection, 1, GL_FALSE, MAT(Camera::getProjection()));
    glUniformMatrix4fv(view, 1, GL_FALSE, MAT(Camera::getView()));
    //ec;
}

void Shader::setMaterial(Material& m)
{
    glUniform1i(uv_tile, m.tile);

    /*std::stringstream ss;   
    for (uint32_t i = 0; i < m.numMaps(); ++i)
    {
        Texture t = m.getMap(i);
        std::string typePrefix = "diffuse";
        if (t.getType() == TexType::DIFFUSE)
        {
            typePrefix = "diffuse";
        }
        else if (t.getType() == TexType::SPECULAR)
        {
            typePrefix = "specular";
        }
        glActiveTexture(GL_TEXTURE0 + i);
        ss << typePrefix << "Map[" << i << "]";
        unsigned int loc = glGetUniformLocation(id, ss.str().c_str());
        ss.clear();
        glUniform1ui(loc, t.getId());
        glBindTexture(GL_TEXTURE_2D, t.getId());
    }*/

    for (uint32_t i = 0; i < m.numMaps(); ++i)
    {
        Texture* t = m.getMap(i);
        GLuint loc;
        if (t->getType() == TexType::DIFFUSE)
        {
            loc = m_diffuseMaps[i];
        }
        else if (t->getType() == TexType::SPECULAR)
        {
            loc = m_specularMaps[i];
        }
        glUniform1i(loc, i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, t->getId());
    }

    glActiveTexture(GL_TEXTURE0);
}

void Shader::setModel(Object& o)
{
    glUniformMatrix4fv(model, 1, GL_FALSE, MAT(o.getWorldMatrix()));
}

}
