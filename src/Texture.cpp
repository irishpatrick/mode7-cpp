#include "Texture.hpp"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "gl.h"

GLuint Texture::open(const std::string& fn)
{
    SDL_Surface* s = IMG_Load(fn.c_str());
    if (s == nullptr)
    {
        std::cout << "failed to load " << fn << std::endl;
        return -1;
    }

    int mode = GL_RGB;
    if (s->format->BytesPerPixel == 4)
    {
        std::cout << "Texture: " << fn << ": mode switch" << std::endl;
        mode = GL_RGBA;
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, s->w, s->h, 0, mode, GL_UNSIGNED_BYTE, s->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(s);

    return id;
}
