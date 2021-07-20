#include "DummyMember.hpp"

DummyMember::DummyMember()
{

}

DummyMember::~DummyMember()
{

}

Box DummyMember::getBoundingBox()
{
    Box bb;
    bb.create(0, 0, 10, 10);
    return bb;
}