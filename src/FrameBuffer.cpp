#include "FrameBuffer.hpp"

#include <cstdlib>
#include <iostream>

namespace mode7
{

static float WMAX = 1.0;
static float screen_quad_data[] = {
    -1.0f, -1.0f,  0.f,  0.f,
     WMAX, -1.0f,  1.f,  0.f,
     WMAX,  WMAX,  1.f,  1.f,

     WMAX,  WMAX,  1.f,  1.f,
    -1.0f,  WMAX,  0.f,  1.f,
    -1.0f, -1.0f,  0.f,  0.f
};

FrameBuffer::FrameBuffer() :
    m_shader(nullptr),
    m_shader_override(nullptr)
{

}

FrameBuffer::~FrameBuffer()
{

}

int FrameBuffer::init(uint32_t w, uint32_t h)
{
    m_width = w;
    m_height = h;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    glGenTextures(1, &m_fb_tex);
    glBindTexture(GL_TEXTURE_2D, m_fb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fb_tex, 0);
    
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "error: fb not complete!" << std::endl;
        return 1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint vbo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), screen_quad_data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}

}