#ifndef DUMMYMEMBER_HPP
#define DUMMYMEMBER_HPP

#include "QTreeMember.hpp"
#include "Box.hpp"


class DummyMember : public QTreeMember
{
public:
    DummyMember();
    ~DummyMember();
    virtual Box getBoundingBox();

private:
};

#endif /* DUMMYMEMBER_HPP */