#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <memory>
#include "Part.hpp"

class Track
{
public:

    Track();
    ~Track();

private:

    std::vector<std::unique_ptr<Part>> m_parts;
};

#endif /* TRACK_HPP */
