#ifndef TRACKDATA_HPP
#define TRACKDATA_HPP

#include <string>
#include <vector>

#include "Line2D.hpp"
#include "Rect.hpp"

namespace mode7
{

    class TrackData
    {
    public:
        TrackData();
        ~TrackData();

        int open(const std::string&);

    private:
        std::vector<Line2D> m_centerLines;
        std::vector<Rect> m_trackBounds;
    };
}

#endif /* TRACKDATA_HPP */