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
            return 1;
        }

        // read num segments
        int32_t n_centerline_pts;
        int32_t n_track_pts;
        int32_t n_runoff_pts;
        int32_t n_wall_pts;

        in.read(buffer, 4 * sizeof(int32_t));
        n_centerline_pts    = *(int32_t*)(buffer + (0 * sizeof(int32_t)));
        n_track_pts         = *(int32_t*)(buffer + (1 * sizeof(int32_t)));
        n_runoff_pts        = *(int32_t*)(buffer + (2 * sizeof(int32_t)));
        n_wall_pts          = *(int32_t*)(buffer + (3 * sizeof(int32_t)));

        // read starting locations
        uint32_t start;
        uint32_t stride;

        in.read(buffer, 2 * sizeof(uint32_t));
        start   = *(uint32_t*)(buffer + (0 * sizeof(uint32_t)));
        stride  = *(uint32_t*)(buffer + (1 * sizeof(uint32_t)));

        // move head to start
        in.seekg(start, std::ios::beg);

        // parse data
        for (int i = 0; i < n_centerline_pts; ++i)
        {

        }

        return 0;
    }

}