#ifndef RECT_HPP
#define RECT_HPP

#include "Line2D.hpp"
#include "gl.hpp"

namespace mode7
{

class Rect
{
public:

    inline void connect(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 pd)
    {
        p[0] = pa;
        p[1] = pb;
        p[2] = pc;
        p[3] = pd;

        a.create(pa, pb);
        b.create(pb, pc);
        c.create(pc, pd);
        d.create(pd, pa);
    }

    inline bool checkIntersect(glm::vec2 pt)
    {
        float da = fabsf(a.distTo(pt));
        float db = fabsf(b.distTo(pt));
        float dc = fabsf(c.distTo(pt));
        float dd = fabsf(d.distTo(pt));

        float l = a.length();
        float w = b.length();
        
        return (da <= w) && (db <= l) && (dc <= w) && (dd <= l);
    }

    Line2D a;
    Line2D b;
    Line2D c;
    Line2D d;
    glm::vec2 p[4];
};

}
#endif /* RECT_HPP */
