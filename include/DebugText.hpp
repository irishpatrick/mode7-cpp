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

#ifdef _BUILD_DEBUG_TOOLS

#ifndef DEBUGTEXT_HPP
#define DEBUGTEXT_HPP

#include "gl.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include <SDL_ttf.h>
#include <string>

namespace mode7
{

class DebugText : public Object
{
public:

    DebugText();
    ~DebugText();

    void init();

    inline void setText(const std::string& n)
    {
        m_text = n;
        renderText();
    }

    inline void destroy()
    {
        if (m_surf)
        {
            SDL_FreeSurface(m_surf);
        }
        if (m_font)
        {
            TTF_CloseFont(m_font);
        }
    }

    void renderText();
    void draw();

private:

    std::string m_text;
    unsigned int m_textureLoc;
    unsigned int m_vao;
    TTF_Font* m_font;
    SDL_Surface* m_surf;
    Texture m_texture;
    Shader m_textShader;
    Mesh m_quad;
};

}

#endif /* DEBUGTEXT_HPP */

#endif /* _BUILD_DEBUG_TOOLS */