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