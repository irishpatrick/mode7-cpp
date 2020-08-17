#ifndef DEBUGPATH_HPP
#define DEBUGPATH_HPP

#include "gl.h"
#include "Object.hpp"
#include "Shader.hpp"
#include <vector>

class DebugPath : public Object
{
public:

    DebugPath();
    virtual ~DebugPath();

    void init();
    void createFromPoints(std::vector<glm::vec2>&);
    void draw();

private:

    bool m_isReady;
    GLuint vao;
    GLuint vbo;
    GLuint num_lines;
    Shader shader;
};

#endif /* DEBUGPATH_HPP */