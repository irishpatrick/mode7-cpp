#ifndef RACINGLINE_HPP
#define RACINGLINE_HPP

#include <string>
#include <vector>
#include "gl.h"
#include "Line2D.hpp"
#include "DebugPath.hpp"

namespace mode7
{

typedef struct _Rect
{
    Line2D a, b, c, d;
} Rect;

static inline int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

class RacingLine
{
public:

    RacingLine();
    ~RacingLine();

    int load(const std::string&);
    int getCurrentIndex(glm::vec2, int);

    inline Line2D getLine(int index)
    {
        return m_lines[index];
    }

    inline Line2D getNext(int index)
    {
        return m_lines[mod(index + 1, m_lines.size())];
    }

    inline DebugPath* getDebugPath()
    {
        return &m_path;
    }

private:

    DebugPath m_path;
    std::vector<Line2D> m_lines;
    std::vector<Rect> m_rects;
};

}

#endif /* RACINGLINE_HPP */