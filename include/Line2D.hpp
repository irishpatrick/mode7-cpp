#ifndef LINE2D_HPP
#define LINE2D_HPP

#include "gl.h"

class Line2D
{
public:

    inline Line2D() {}
    inline ~Line2D() {}

    inline void create(glm::vec2 a, glm::vec2 b)
    {
        m_p = a;
        m_e = a;
        m_v = b;
    }

    inline void fromPoints(glm::vec2 a, glm::vec2 b)
    {
        m_v = b - a;
        m_p = a;
        m_e = b;
    }

    inline glm::vec2 normal()
    {
        return glm::vec2(-m_v.y, m_v.x);
    }

    inline glm::vec2 solve(float t)
    {
        return m_p + (t * m_v);
    }

    inline float distTo(glm::vec2 pt)
    {
        return glm::dot((pt - m_p), normal());
    }

    inline float length()
    {
        return glm::length(m_e - m_p);
    }

    inline glm::vec2 p()
    {
        return m_p;
    }

    inline glm::vec2 e()
    {
        return m_e;
    }

private:

    glm::vec2 m_p;
    glm::vec2 m_e;
    glm::vec2 m_v;
};

#endif /* LINE2D_HPP */