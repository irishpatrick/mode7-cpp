#include "Texture.hpp"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cassert>
#include "gl.hpp"

namespace mode7
{

    Texture::Texture() :
        type(TexType::DIFFUSE),
        id(-1),
        m_pixels(nullptr)
    {
    }

    Texture::~Texture()
    {
        destroy();
    }

    void Texture::open(const std::string& fn, TexType type)
    {
        destroy();
        this->type = type;
        open(fn);
        addSrcRect(0, 0, m_pixels->w, m_pixels->h);
    }

    uint32_t Texture::getId()
    {
        return id;
    }

    TexType Texture::getType()
    {
        return type;
    }

    void Texture::fill(void* data, int w, int h)
    {
        destroy();

        this->type = TexType::DIFFUSE;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    int Texture::open(const std::string& fn)
    {
        std::cout << "load texture " << fn << std::endl;
        assert(m_pixels == nullptr);
        m_pixels = IMG_Load(fn.c_str());
        if (m_pixels == nullptr)
        {
            std::cout << "failed to load " << fn << std::endl;
            return -1;
        }

        int mode = GL_RGB;
        if (m_pixels->format->BytesPerPixel == 4)
        {
            mode = GL_RGBA;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, m_pixels->w, m_pixels->h, 0, mode, GL_UNSIGNED_BYTE, m_pixels->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        return 0;
    }

    void Texture::addSrcRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
    {
        SDL_Rect rect = {x, y, w, h};
        m_srcRects.push_back(rect);
    }

    void Texture::useSrcRect(uint32_t indx)
    {
        if (!id)
        {
            return;
        }

        SDL_Rect* r = &m_srcRects.back();

        glBindTexture(GL_TEXTURE_2D, id);
        int mode = GL_RGB;
        if (m_pixels->format->BytesPerPixel == 4)
        {
            mode = GL_RGBA;
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, r->x, r->y, r->w, r->h, mode, GL_UNSIGNED_BYTE, m_pixels->pixels);
    }

}
