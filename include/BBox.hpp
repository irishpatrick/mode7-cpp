#ifndef BBOX_HPP
#define BBOX_HPP

#include <glm/glm.hpp>

class BBox
{
public:

    BBox();
    ~BBox();

    bool intersects(BBox&);

private:
    glm::vec3 pos;
    glm::vec3 dim;
};

#endif /* BBOX_HPP */
