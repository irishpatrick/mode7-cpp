#include "DebugText.hpp"

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
}

void DebugText::renderText()
{

}

void DebugText::draw()
{
    m_textShader.onlyUse();
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(m_textureLoc, m_texture.getId());
    glBindTexture(GL_TEXTURE_2D, m_texture.getId());
    glBindVertexArray(m_vao);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glActiveTexture(0);
    glBindVertexArray(0);
}

}