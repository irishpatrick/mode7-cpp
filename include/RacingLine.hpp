/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef RACINGLINE_HPP
#define RACINGLINE_HPP

#include <string>
#include <vector>
#include "gl.hpp"
#include "Line2D.hpp"
#include "DebugPath.hpp"
#include "Rect.hpp"
#include <cassert>

namespace mode7
{

/*typedef struct _Rect
{
    Line2D a, b, c, d;
} Rect;*/

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
        return m_lines[index].first;
    }

    inline Rect getRect(int index)
    {
        return m_bounds[index];
    }

    inline int getNextIndex(int index)
    {
        return (index + 1) % m_lines.size();
    }

    inline int getPrevIndex(int index)
    {
        return (index - 1) % m_lines.size();
    }

    inline Line2D getNext(int index)
    {
        //return m_lines[mod(index + 1, m_lines.size())];
        assert(index >= 0);
        int next = (index + 1) % m_lines.size();
        assert(next >= 0);
        return m_lines[next].first;
    }

    /*inline bool checkBounds(int index)
    {
        Rect* r = &m_rects[index];
    }*/

#ifdef _BUILD_DEBUG_TOOLS
    inline DebugPath* getDebugPath()
    {
        return &m_path;
    }
#endif /* _BUILD_DEBUG_TOOLS */

private:

    int m_queuedAction;
#ifdef _BUILD_DEBUG_TOOLS
    DebugPath m_path;
#endif /* _BUILD_DEBUG_TOOLS */
    std::vector<std::pair<Line2D, int>> m_lines;
    std::vector<Rect> m_rects;
    std::vector<Rect> m_bounds;
};

}

#endif /* RACINGLINE_HPP */
