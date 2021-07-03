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
            std::cout << m_pixels << std::endl;
            if (m_pixels)
            {
                std::cout << "freesurf " << m_pixels << std::endl;
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
