#ifndef SHADER_HPP
#define SHADER_HPP

#include "gl.h"
#include "Material.hpp"
#include "Object.hpp"
#include <string>

class Shader
{
public:

    Shader();
    ~Shader();

    int open(const std::string&, const std::string&);

    void use();
    void setMaterial(Material&);
    void setModel(Object&);

private:

    GLuint id;
    GLuint projection;
    GLuint view;
    GLuint model;
    GLuint diffuseTexture;
};


#endif /* SHADER_HPP */