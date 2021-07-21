#include "TrackData.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cstdint>

namespace mode7
{

    TrackData::TrackData()
    {

    }

    TrackData::~TrackData()
    {

    }

    int TrackData::open(const std::string& fn)
    {
        std::cout << "[TrackData] open " << fn << std::endl;

        std::ifstream in;
        in.open(fn, std::ios::binary);
        
        size_t length;
        in.seekg(0, std::ios::end);
        length = in.tellg();
        in.seekg(0, std::ios::beg);

        if (length > 1024 * 1024 * 1024)
        {
            // too big
            return 1;
        }

        char buffer[1024];
        memset(buffer, 0, 1024);

        // check magic word
        in.read(buffer, 4);
        if (strcmp(buffer, "TDAT") != 0)
        {
            return 2;
        }

        // read num segments
        int32_t n_centerline_pts;
        int32_t n_track_pts;
        int32_t n_runoff_pts;
        int32_t n_wall_pts;

        in.read(buffer, 1 * sizeof(int32_t));
        n_centerline_pts    = *(int32_t*)(buffer + (0 * sizeof(int32_t)));
        //n_track_pts         = *(int32_t*)(buffer + (1 * sizeof(int32_t)));
        //n_runoff_pts        = *(int32_t*)(buffer + (2 * sizeof(int32_t)));
        //n_wall_pts          = *(int32_t*)(buffer + (3 * sizeof(int32_t)));

        // read starting locations
        uint32_t start;
        uint32_t stride;

        in.read(buffer, 2 * sizeof(uint32_t));
        start   = *(uint32_t*)(buffer + (0 * sizeof(uint32_t)));
        stride  = *(uint32_t*)(buffer + (1 * sizeof(uint32_t)));

        if (stride != 28 * sizeof(float))
        {
            // bad format
            // return 3;
        }

        // move head to start
        //in.seekg(start, std::ios::beg);

        // parse data
        float qbuf[8];
        line ln;
        quad qd;
        for (int i = 0; i < n_centerline_pts; ++i)
        {
            // read centerline
            in.read((char*)qbuf, 4 * sizeof(float));
            //std::cout << "buffer " << qbuf[0] << "," << qbuf[1] << "," << qbuf[2] << "," << qbuf[3] << std::endl;
            line_from_buffer(&ln, qbuf);
            m_centerLines.push_back(ln);
            //std::cout << "line " << ln.p1[0] << "," << ln.p1[1] << "," << ln.p2[0] << "," << ln.p2[1] << std::endl;

            // read track bounds
            in.read((char*)qbuf, 8 * sizeof(float));
            quad_from_buffer(&qd, qbuf);
            m_trackBounds.push_back(qd);

            // read runoff bounds
            in.read((char*)qbuf, 8 * sizeof(float));
            quad_from_buffer(&qd, qbuf);
            m_runoffBounds.push_back(qd);

            // read wall bounds
            in.read((char*)qbuf, 8 * sizeof(float));
            quad_from_buffer(&qd, qbuf);
            m_wallBounds.push_back(qd);
        }

        return 0;
    }

    int TrackData::transform(const glm::mat4& matrix)
    {
        std::cout << "[TrackData] transform" << std::endl;
     
        assert(m_centerLines.size() > 0);
     
        int n_processed = 0;

        line* cur_line;
        quad* cur_quad;
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;
        glm::vec3 d;
        for (uint32_t i = 0; i < m_centerLines.size(); ++i)
        {
            cur_line = &m_centerLines[i];
            a = glm::vec3(cur_line->p1[0], 0, cur_line->p1[1]);
            b = glm::vec3(cur_line->p2[0], 0, cur_line->p2[1]);
            a = glm::vec3(matrix * glm::vec4(a, 1));
            b = glm::vec3(matrix * glm::vec4(b, 1));
            line_connect(cur_line, a.x, a.z, b.x, b.z);

            cur_quad = &m_trackBounds[i];
            a = glm::vec3(cur_quad->p[0].x, 0, cur_quad->p[0].y);
            b = glm::vec3(cur_quad->p[1].x, 0, cur_quad->p[1].y);
            c = glm::vec3(cur_quad->p[2].x, 0, cur_quad->p[2].y);
            d = glm::vec3(cur_quad->p[3].x, 0, cur_quad->p[3].y);
            a = glm::vec3(matrix * glm::vec4(a, 1));
            b = glm::vec3(matrix * glm::vec4(b, 1));
            c = glm::vec3(matrix * glm::vec4(c, 1));
            d = glm::vec3(matrix * glm::vec4(d, 1));
            quad_connect_raw(
                cur_quad,
                &(glm::vec2(a.x, a.z))[0],
                &(glm::vec2(b.x, b.z))[0],
                &(glm::vec2(c.x, c.z))[0],
                &(glm::vec2(d.x, d.z))[0]
            );

            cur_quad = &m_runoffBounds[i];
            a = glm::vec3(cur_quad->p[0].x, 0, cur_quad->p[0].y);
            b = glm::vec3(cur_quad->p[1].x, 0, cur_quad->p[1].y);
            c = glm::vec3(cur_quad->p[2].x, 0, cur_quad->p[2].y);
            d = glm::vec3(cur_quad->p[3].x, 0, cur_quad->p[3].y);
            a = glm::vec3(matrix * glm::vec4(a, 1));
            b = glm::vec3(matrix * glm::vec4(b, 1));
            c = glm::vec3(matrix * glm::vec4(c, 1));
            d = glm::vec3(matrix * glm::vec4(d, 1));
            quad_connect_raw(
                cur_quad,
                &(glm::vec2(a.x, a.z))[0],
                &(glm::vec2(b.x, b.z))[0],
                &(glm::vec2(c.x, c.z))[0],
                &(glm::vec2(d.x, d.z))[0]
            );
            
            cur_quad = &m_wallBounds[i];
            a = glm::vec3(cur_quad->p[0].x, 0, cur_quad->p[0].y);
            b = glm::vec3(cur_quad->p[1].x, 0, cur_quad->p[1].y);
            c = glm::vec3(cur_quad->p[2].x, 0, cur_quad->p[2].y);
            d = glm::vec3(cur_quad->p[3].x, 0, cur_quad->p[3].y);
            a = glm::vec3(matrix * glm::vec4(a, 1));
            b = glm::vec3(matrix * glm::vec4(b, 1));
            c = glm::vec3(matrix * glm::vec4(c, 1));
            d = glm::vec3(matrix * glm::vec4(d, 1));
            quad_connect_raw(
                cur_quad,
                &(glm::vec2(a.x, a.z))[0],
                &(glm::vec2(b.x, b.z))[0],
                &(glm::vec2(c.x, c.z))[0],
                &(glm::vec2(d.x, d.z))[0]
            );
        }

        return n_processed;
    }

    std::vector<glm::vec2> TrackData::getCenterLinePts()
    {
        std::vector<glm::vec2> points;
        line* cur;

        for (uint32_t i = 0; i < m_centerLines.size(); ++i)
        {
            cur = &m_centerLines[i];
            points.push_back(glm::vec2(cur->p1[0], cur->p1[1]));
            points.push_back(glm::vec2(cur->p2[0], cur->p2[1]));
        }

        return points;
    }

    std::vector<glm::vec2> TrackData::getTrackBoundPts()
    {
        std::vector<glm::vec2> points;
        quad* cur;
        uint32_t a;
        uint32_t b;

        for (uint32_t i = 0; i < m_trackBounds.size(); ++i)
        {
            cur = &m_trackBounds[i];
            for (uint32_t j = 0; j < 4; ++j)
            {
                a = (j + 0) % 4;
                b = (j + 1) % 4;
                points.push_back(glm::vec2(cur->p[a].x, cur->p[a].y));
                points.push_back(glm::vec2(cur->p[b].x, cur->p[b].y));
            }
        }

        return points;
    }

    std::vector<glm::vec2> TrackData::getRunoffBoundPts()
    {
        std::vector<glm::vec2> points;
        quad* cur;
        uint32_t a;
        uint32_t b;

        for (uint32_t i = 0; i < m_runoffBounds.size(); ++i)
        {
            cur = &m_runoffBounds[i];
            for (uint32_t j = 0; j < 4; ++j)
            {
                a = (j + 0) % 4;
                b = (j + 1) % 4;
                points.push_back(glm::vec2(cur->p[a].x, cur->p[a].y));
                points.push_back(glm::vec2(cur->p[b].x, cur->p[b].y));
            }
        }

        return points;
    }

    std::vector<glm::vec2> TrackData::getWallBoundPts()
    {
        std::vector<glm::vec2> points;
        quad* cur;
        uint32_t a;
        uint32_t b;

        for (uint32_t i = 0; i < m_wallBounds.size(); ++i)
        {
            cur = &m_wallBounds[i];
            for (uint32_t j = 0; j < 4; ++j)
            {
                a = (j + 0) % 4;
                b = (j + 1) % 4;
                points.push_back(glm::vec2(cur->p[a].x, cur->p[a].y));
                points.push_back(glm::vec2(cur->p[b].x, cur->p[b].y));
            }
        }

        return points;
    }
}