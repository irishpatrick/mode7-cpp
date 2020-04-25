#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "gl.h"

class Material
{
public:

    Material();
    ~Material();

    void setDiffuseTexture(GLuint);

    GLuint getDiffuseTexture();

private:

    GLuint diffuseTex;
};

#endif /* MATERIAL_HPP */