#include "Track.hpp"
#include "Tree.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ModelLoader.hpp"
#include "Car.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;

namespace mode7
{
    static inline Rect quad_to_rect(quad* q)
    {
        Rect out;
        out.connect(
            glm::vec2(q->p[0].x, q->p[0].y),
            glm::vec2(q->p[1].x, q->p[1].y),
            glm::vec2(q->p[2].x, q->p[2].y),
            glm::vec2(q->p[3].x, q->p[3].y)
            // glm::vec2(q->p[3].x, q->p[3].y),
            // glm::vec2(q->p[2].x, q->p[2].y),
            // glm::vec2(q->p[1].x, q->p[1].y),
            // glm::vec2(q->p[0].x, q->p[0].y)
        );
        return out;
    }

    Track::Track()
    {

    }

    Track::~Track()
    {

    }

    void Track::open(const std::string& fn)
    {
        m_scene = std::move(ModelLoader::openUnique(fn));
    }

    void Track::attachData(const std::string& fn)
    {
        int err = m_data.open(fn);
        if (err)
        {
            std::cout << "[Track] error " << err << ": failed to open " << fn << std::endl;
            return;
        }
        m_data.transform(m_scene->getWorldMatrix());

        genTrackZones();

#ifdef _BUILD_DEBUG_TOOLS
     
        m_centerLineDbg.init();
        m_centerLineDbg.setColor(1.0, 1.0, 1.0);
        m_trackBoundDbg.init();
        m_trackBoundDbg.setColor(0.0, 1.0, 0.0);
        m_runoffBoundDbg.init();
        m_runoffBoundDbg.setColor(1.0, 1.0, 0.0);
        m_wallBoundDbg.init();
        m_wallBoundDbg.setColor(1.0, 0.0, 0.0);
    
        std::vector<glm::vec2> pts;

        pts = m_data.getCenterLinePts();
        assert(pts.size() > 0);
        m_centerLineDbg.createFromPoints(pts);
        assert(m_centerLineDbg.isReady());

        pts = m_data.getTrackBoundPts();
        assert(pts.size() > 0);
        m_trackBoundDbg.createFromPoints(pts);
        assert(m_trackBoundDbg.isReady());

        pts = m_data.getRunoffBoundPts();
        assert(pts.size() > 0);
        m_runoffBoundDbg.createFromPoints(pts);
        assert(m_runoffBoundDbg.isReady());

        pts = m_data.getWallBoundPts();
        assert(pts.size() > 0);
        m_wallBoundDbg.createFromPoints(pts);
        assert(m_wallBoundDbg.isReady());
        
#endif /* _BUILD_DEBUG_TOOLS */
    }

    void Track::genTrackZones()
    {
        assert(m_data.getNumZones() > 0);

        m_zones.reserve(m_data.getNumZones());

        TrackZone tz;
        for (uint32_t i = 0; i < m_data.getNumZones(); ++i)
        {
            tz.m_trackRect = quad_to_rect(&m_data.getTrackBounds()[i]);
            tz.m_runoffRect = quad_to_rect(&m_data.getRunoffBounds()[i]);
            tz.m_wallRect = quad_to_rect(&m_data.getWallBounds()[i]);
            m_zones.push_back(tz);
        }
    }

    void Track::placeCarOnGrid(Car* car, uint32_t pos)
    {
        auto centerLines = m_data.getCenterLines();
        float ox = centerLines[0].p1[0];
        float oy = centerLines[0].p1[1];
        float dx = 1;
        float dy = 1.5;

        int side = (pos + 1) % 2;

        car->position.x = ox + dx * side;
        car->position.z = oy + dy * (pos / 2);
        
    }

    Scene* Track::getScene()
    {
        return m_scene.get();
    }

    void Track::update()
    {
#ifdef _BUILD_DEBUG_TOOLS
     
        m_centerLineDbg.update();
        m_trackBoundDbg.update();
        m_runoffBoundDbg.update();
        m_wallBoundDbg.update();
    
#endif /* _BUILD_DEBUG_TOOLS */
    }

    void Track::draw(Shader& s)
    {
        m_scene->draw(s);

#ifdef _BUILD_DEBUG_TOOLS

        glDisable(GL_DEPTH_TEST);
        m_wallBoundDbg.draw();
        m_runoffBoundDbg.draw();
        m_trackBoundDbg.draw();
        m_centerLineDbg.draw();        
        glEnable(GL_DEPTH_TEST);

#endif /* _BUILD_DEBUG_TOOLS */
    }

    void Track::destroy()
    {
    }

    int32_t pymod(int32_t n, int32_t m)
    {
        return ((n % m) + m) % m;
    }

    std::vector<std::pair<uint32_t, TrackZone*>> Track::getNearbyZones(uint32_t curZone)
    {
        std::vector<std::pair<uint32_t, TrackZone*>> out;
        uint32_t back;
        uint32_t front;
        uint32_t d = 20;

        out.reserve(d * 2);

        // (b + (a%b)) % b
        // ((-1 % 10) + 10) % 10
        //back = (m_data.getNumZones() + ((int32_t)curZone - d) % m_data.getNumZones()) % m_data.getNumZones();
        back = pymod((int32_t)curZone - (int32_t)d * 3 / 2, m_data.getNumZones());
        front = ((int32_t)curZone + (int32_t)d / 2) % m_data.getNumZones();

        //std::cout << "cur=" << curZone << "\tback=" << back << "\tfront=" << front  << "\tnzones=" << m_data.getNumZones() << std::endl;

        uint32_t i = front;
        while (i != back)
        {
            out.push_back(std::pair<uint32_t, TrackZone*>(i, &m_zones[i]));
            //i = (m_data.getNumZones() + ((int32_t)i - 1) % m_data.getNumZones()) % m_data.getNumZones();
            i = pymod((int32_t)i - 1, m_data.getNumZones());
        }

        assert(out.size() == d * 2);

        return out;
    }
}