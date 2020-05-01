#ifndef JOINT_HPP
#define JOINT_HPP

#include <glm/glm.hpp>

class Piece;

class Joint
{
public:

    Joint();
    ~Joint();

    void attach(Joint&);
    void align(Joint&);
    void flipNormal();

    glm::vec2 offset;
    glm::vec2 normal;

private:

    Joint* connection;
    Piece* parent;
};

#endif /* JOINT_HPP */