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
}

}