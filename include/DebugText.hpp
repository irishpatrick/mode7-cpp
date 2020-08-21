#ifndef DEBUGTEXT_HPP
#define DEBUGTEXT_HPP

#include "gl.h"
#include "Shader.hpp"
#include "Object.hpp"
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

    void renderText();
    void draw();

private:

    std::string m_text;
    unsigned int m_texId;
    unsigned int vao;
    Shader m_textShader;
};

}

#endif /* DEBUGTEXT_HPP */