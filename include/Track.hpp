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

namespace mode7
{
    class Track
    {
    public:

        Track();
        ~Track();

        void open(const std::string&);
        void attachData(const std::string&);
        Scene* getScene();
        void update();
        void draw(Shader&);
        void destroy();

    private:

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
