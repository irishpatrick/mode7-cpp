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

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "gl.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <SDL.h>

namespace mode7
{

    enum class TexType {DIFFUSE, SPECULAR, NORMAL, BUMP};

    class Texture
    {
    public:

        Texture();
        ~Texture();

        void open(const std::string&, TexType);
        int open(const std::string&);
        void fill(void*, int, int);
        void addSrcRect(uint32_t, uint32_t, uint32_t, uint32_t);
        void useSrcRect(uint32_t);
        uint32_t getId();
        TexType getType();

        inline void destroy()
        {
            //std::cout << m_pixels << std::endl;
            if (m_pixels)
            {
                //std::cout << "freesurf " << m_pixels << std::endl;
                SDL_FreeSurface(m_pixels);
                m_pixels = nullptr;
            }
            if ((GLint)id != 0)
            {
                glDeleteTextures(1, &id);
                id = 0;
            }
        }

        // static methods
    private:

        TexType type;
        GLuint id;

        SDL_Surface* m_pixels;
        std::vector<SDL_Rect> m_srcRects;
    };

}

#endif /* TEXTURE_HPP */
