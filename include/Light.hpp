#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Object.hpp"
#include "gl.h"

class Light : public Object
{
public:

    Light();
    ~Light();

    void create(int);

    // colors
    float intensity;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // attenuation
    float contant;
    float linear;
    float quadratic;

    static const int AMBIENT = 0;
    static const int DIRECTIONAL = 1;
    static const int SPECULAR = 2;

private:

    int type;
};

#endif /* LIGHT_HPP */
