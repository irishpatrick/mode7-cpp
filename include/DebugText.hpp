#ifndef DEBUGTEXT_HPP
#define DEBUGTEXT_HPP

#include "gl.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
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
        //renderText();
    }

    void renderText();
    void draw();

private:

    std::string m_text;
    unsigned int m_textureLoc;
    unsigned int m_vao;
    Texture m_texture;
    Shader m_textShader;
    Mesh m_quad;
};

}

#endif /* DEBUGTEXT_HPP */