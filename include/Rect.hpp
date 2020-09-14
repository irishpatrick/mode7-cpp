#ifndef RECT_HPP
#define RECT_HPP

#include "Line2D.hpp"
#include "gl.h"

namespace mode7
{

class Rect
{
public:

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
};

}
#endif /* RECT_HPP */
