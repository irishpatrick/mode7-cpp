#ifndef QTREEMEMBER_HPP
#define QTREEMEMBER_HPP

#include "Box.hpp"

class QTreeMember
{
public:
    virtual Box getBoundingBox() = 0;
};

#endif /* QTREEMEMBER_HPP */