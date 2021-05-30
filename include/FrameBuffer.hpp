#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "gl.h"
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
    
    inline GLuint getTexGL()
    {
        return m_fb_tex;
    }

private:

    GLuint m_fbo;
    GLuint m_rbo;
    GLuint m_fb_tex;
    GLuint m_vao;
    Shader m_shdr;
};

}

#endif /* FRAMEBUFFER_HPP */