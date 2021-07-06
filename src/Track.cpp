#include "Track.hpp"
#include "Tree.hpp"
#include "Object.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ModelLoader.hpp"

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
        //m_data.transform(m_scene->getWorldMatrix());

#ifdef _BUILD_DEBUG_TOOLS
     
        m_centerLineDbg.init();
        m_trackBoundDbg.init();
        m_runoffBoundDbg.init();
        m_wallBoundDbg.init();
    
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

        m_centerLineDbg.draw();
        //m_trackBoundDbg.draw();
        //m_runoffBoundDbg.draw();
        //m_wallBoundDbg.draw();

#endif /* _BUILD_DEBUG_TOOLS */
    }

    void Track::destroy()
    {
    }
}