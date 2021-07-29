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

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "gl.hpp"
#include "Shader.hpp"

#include <cstdint>

namespace mode7
{

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    int init(uint32_t w, uint32_t h);
    
    inline void setShader(Shader* sh)
    {
        m_shader = sh;
        m_colortex_loc = glGetUniformLocation(m_shader->pid(), "colorTex");
    }

    inline void forceShader(Shader* sh)
    {
        m_shader_override = sh;
    }

    inline Shader* getScreenShader()
    {
        return m_shader;
    }

    inline Shader* getForcedShader()
    {
        return m_shader_override;
    }
    
    inline GLuint getTexGL()
    {
        return m_fb_tex;
    }

    inline void begin()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_width, m_height);
    }

    inline void feedTex(GLint loc, GLuint id)
    {
        glUniform1i(loc, m_tp);
        glActiveTexture(GL_TEXTURE0 + m_tp);
        glBindTexture(GL_TEXTURE_2D, id);
        ++m_tp;
    }

    inline void preDraw()
    {
        m_tp = 0;
        m_shader->onlyUse();
        feedTex(m_colortex_loc, m_fb_tex);
    }

    inline void draw()
    {
        glActiveTexture(GL_TEXTURE0); // not sure if needed

        glBindVertexArray(m_vao);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glActiveTexture(0);
        glBindVertexArray(0);
    }

    inline void destroy()
    {
        glDeleteFramebuffers(1, &m_fbo);
    }

private:

    GLuint m_fbo;
    GLuint m_rbo;
    GLuint m_fb_tex;
    GLuint m_vao;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_tp;
    Shader* m_shader;
    GLuint m_colortex_loc;
    Shader* m_shader_override;
};

}

#endif /* FRAMEBUFFER_HPP */