#ifndef PIECE_HPP
#define PIECE_HPP

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include "Joint.hpp"

class Piece
{
public:

    Piece();
    ~Piece();
    
    void lock();
    void unlock();
    bool isLocked();
    void setAngle(float);
    void addJoint(uint32_t, uint32_t);

    Joint& getJoint(uint32_t);

private:
    
    float rotation;
    bool rotation_locked;

    glm::vec2 origin;
    std::vector<glm::vec2> shape;
    std::vector<Joint> joints;
};

#endif /* PIECE_HPP */