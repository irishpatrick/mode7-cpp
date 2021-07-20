#include "Track.hpp"
#include "Tree.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ModelLoader.hpp"
#include "Car.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace mode7
{

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

    void Track::placeCarOnGrid(Car* car, uint32_t pos)
    {
        auto clp = m_data.getCenterLinePts();
        float ox = clp[0].x;
        float oy = clp[0].y;
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

    std::vector<TrackZone*> Track::getNearbyZones(uint32_t curZone)
    {
        std::vector<TrackZone*> out;

        return out;
    }
}