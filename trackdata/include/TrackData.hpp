#ifndef TRACKDATA_HPP
#define TRACKDATA_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "geometry.h"
#include "line.h"

namespace mode7
{

    class TrackData
    {
    public:
        TrackData();
        ~TrackData();

        int open(const std::string&);
        int transform(const glm::mat4&);
        std::vector<glm::vec2> getCenterLinePts();
        std::vector<glm::vec2> getTrackBoundPts();
        std::vector<glm::vec2> getRunoffBoundPts();
        std::vector<glm::vec2> getWallBoundPts();

    private:
        std::vector<line> m_centerLines;
        std::vector<quad> m_trackBounds;
        std::vector<quad> m_runoffBounds;
        std::vector<quad> m_wallBounds;
    };
}

#endif /* TRACKDATA_HPP */