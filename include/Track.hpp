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
