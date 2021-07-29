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

#ifndef TRACK_HPP
#define TRACK_HPP

#include "Shader.hpp"
#include "Line2D.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include "TrackData.hpp"
#include "DebugPath.hpp"

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#define ZONE_TRACK 0x01
#define ZONE_RUNOFF 0x02
#define ZONE_WALL 0x04

#define IS_IN_ZONE(x, y) (x & y)

namespace mode7
{
    // Forward declarations
    class Car;

    class TrackZone
    {
    public:
        TrackZone()
        {

        }

        ~TrackZone()
        {

        }

        inline bool onTrack(glm::vec2 v)
        {
            return m_trackRect.checkIntersect(v);
        }

        inline bool onRunoff(glm::vec2 v)
        {
            return m_runoffRect.checkIntersect(v);
        }

        inline bool onWall(glm::vec2 v)
        {
            return m_wallRect.checkIntersect(v);
        }

    private:
        Rect m_trackRect;
        Rect m_runoffRect;
        Rect m_wallRect;

        friend class Track;
    };

    class Track
    {
    public:

        Track();
        ~Track();

        void open(const std::string&);
        void attachData(const std::string&);
        void genTrackZones();
        Scene* getScene();
        void placeCarOnGrid(Car*, uint32_t);
        void update();
        void draw(Shader&);
        void destroy();
        std::vector<std::pair<uint32_t, TrackZone*>> getNearbyZones(uint32_t);

    private:

        std::vector<TrackZone> m_zones;

        std::vector<Line2D> m_centerline;
        std::vector<Rect> m_trackBounds;
        std::vector<Rect> m_runoffBounds;
        std::vector<Rect> m_wallsBounds;
        std::unique_ptr<Scene> m_scene;
        TrackData m_data;
        
#ifdef _BUILD_DEBUG_TOOLS
        DebugPath m_centerLineDbg;
        DebugPath m_trackBoundDbg;
        DebugPath m_runoffBoundDbg;
        DebugPath m_wallBoundDbg;
#endif /* _BUILD_DEBUG_TOOLS */
    };
}

#endif /* TRACK_HPP */
