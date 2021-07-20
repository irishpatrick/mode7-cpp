#ifndef BOX_HPP
#define BOX_HPP

#include <vector>

class Box
{
public:
    Box();
    ~Box();

    void create(float, float, float, float);
    void center(float, float, float, float);
    bool intersects(Box);

private:
    float m_x;
    float m_y;
    float m_w;
    float m_h;
};

#endif /* BOX_HPP */