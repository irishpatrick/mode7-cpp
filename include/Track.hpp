#ifndef TRACK_HPP
#define TRACK_HPP

#include <string>
#include "Shader.hpp"

class Track
{
public:

    static void open(const std::string&);
    static void update();
    static void draw(Shader&);
    static void destroy();
};

#endif /* TRACK_HPP */
