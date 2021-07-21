#ifndef TRACKDATA_HPP
#define TRACKDATA_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <cstdint>

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

        inline std::vector<line>& getCenterLines()
        {
            return m_centerLines;
        }
        
        inline std::vector<quad>& getTrackBounds()
        {
            return m_trackBounds;
        }
        
        inline std::vector<quad>& getRunoffBounds()
        {
            return m_runoffBounds;
        }

        inline std::vector<quad>& getWallBounds()
        {
            return m_wallBounds;
        }

        inline uint32_t getNumZones()
        {
            return m_centerLines.size();
        }

    private:
        std::vector<line> m_centerLines;
        std::vector<quad> m_trackBounds;
        std::vector<quad> m_runoffBounds;
        std::vector<quad> m_wallBounds;
    };
}

#endif /* TRACKDATA_HPP */