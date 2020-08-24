#include "DebugText.hpp"
#include <SDL_ttf.h>
#include <iostream>

static float debugtext_quad_data[] = {
    -1.0f, -1.0f,  0.f,  0.f,
     1.0f, -1.0f,  1.f,  0.f,
     1.0f,  1.0f,  1.f,  1.f,

     1.0f,  1.0f,  1.f,  1.f,
    -1.0f,  1.0f,  0.f,  1.f,
    -1.0f, -1.0f,  0.f,  0.f
};

namespace mode7
{

DebugText::DebugText()
{

}

DebugText::~DebugText()
{

}

void DebugText::init()
{
    m_textShader.open("assets/shaders/text_v.glsl", "assets/shaders/text_f.glsl");
    m_textureLoc = glGetUniformLocation(m_textShader.pid(), "tex");
    
    unsigned int vbo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 4 * sizeof(float), debugtext_quad_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2,
        GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );

    setText("hello world\nnewline\ndata\ndata\ndata");
    renderText();

    float sc = 0.2f;
    this->position.x = -0.5f;
    this->position.y = 0.7f;
    this->scale = glm::vec3(sc);
    this->scale.x *= 16.f / 9.f;
    this->scale.y *= -1.f;
    this->update();
}

void DebugText::renderText()
{
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 24);
    if (!font)
    {
        std::cout << "bad font: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(font, m_text.c_str(), {200, 200, 200}, 400);
    if (!surf)
    {
        std::cout << "bad surface" << std::endl;
        return;
    }
    m_texture.fill(surf->pixels, surf->w, surf->h);
    SDL_FreeSurface(surf);
}

void DebugText::draw()
{
    m_textShader.onlyUse();
    m_textShader.setModel(*this);

    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(m_textureLoc, m_texture.getId());
    glBindTexture(GL_TEXTURE_2D, m_texture.getId());
    
    glBindVertexArray(m_vao);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glActiveTexture(0);
    glBindVertexArray(0);
}

}