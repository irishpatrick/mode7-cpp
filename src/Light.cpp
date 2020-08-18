#include "Light.hpp"

namespace mode7
{

Light::Light() :
    Object(),
    type(-1)
{

}

Light::~Light()
{

}

void Light::create(int type)
{
    this->type = type;
}

}