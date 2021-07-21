#ifdef _BUILD_DEBUG_TOOLS

#ifndef DEBUGPATH_HPP
#define DEBUGPATH_HPP

#include "gl.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include <vector>

namespace mode7
{

class DebugPath : public Object
{
public:

    DebugPath();
    virtual ~DebugPath();

    void init();
    void createFromPoints(std::vector<glm::vec2>&);
    void draw();

    inline bool isReady()
    {
        return m_isReady;
    }

    inline void setColor(float r, float g, float b)
    {
        m_color = glm::vec3(r, g, b);
    }

private:

    bool m_isReady;
    glm::vec3 m_color;
    GLuint vao;
    GLuint vbo;
    GLuint num_lines;
    GLuint m_colorUniform;
    Shader shader;
};

}

#endif /* DEBUGPATH_HPP */

#endif /* _BUILD_DEBUG_TOOLS */