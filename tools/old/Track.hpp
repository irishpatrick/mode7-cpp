#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include "Piece.hpp"

class Track
{
public:

    Track();
    ~Track();

private:

    std::vector<Piece> pieces;
};


#endif /* TRACK_HPP */