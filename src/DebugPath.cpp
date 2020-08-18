#include "DebugPath.hpp"

namespace mode7
{

DebugPath::DebugPath() :
    Object(),
    m_isReady(false),
    vao(-1),
    vbo(-1),
    num_lines(0)
{

}

DebugPath::~DebugPath()
{

}

void DebugPath::init()
{
    shader.open("assets/shaders/debugpath_v.glsl", "assets/shaders/debugpath_f.glsl");
}

void DebugPath::createFromPoints(std::vector<glm::vec2>& points)
{
    m_isReady = true;
    std::vector<glm::vec3> converted;
    converted.reserve(points.size());
    for (auto& e : points)
    {
        converted.push_back(glm::vec3(e.x, 0.f, e.y));
    }
    num_lines = converted.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, converted.size() * sizeof(glm::vec3), &converted[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
}

void DebugPath::draw()
{
    if (m_isReady)
    {
        shader.use();
        shader.setModel(*this);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, num_lines);
    }
}

}