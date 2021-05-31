#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "FrameBuffer.hpp"

#include <cstdint>
#include <string>

namespace mode7
{

class Screen
{
public:

    static void create(int, int, bool);
    static void clear();
    static void addFrameBuffer(const std::string&, FrameBuffer*);
    static const std::vector<FrameBuffer*>& getPipeline();
    static void runPipeline(void (*)(int32_t));
    static void beginRender();
    static void flip();
    static void destroy();

private:

};

#endif /* SCREEN_HPP */

}