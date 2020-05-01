#ifndef MESH_HPP
#define MESH_HPP

#include "Object.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "gl.h"

class Mesh : public Object
{
public:
    
    Mesh();
    virtual ~Mesh();

    void init();
    void setMaterial(Material&);
    void createFromBuffer(const float*, GLuint);
    void createFromShape(int);
    virtual void draw(Shader&);

    void setAltShader(Shader&);

    Material material;

    static const int PLANE = 0;

protected:

    void drawTriangles();

    Shader* alt;

private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    bool elements;
    int triangles;
};

#endif /* MESH_HPP */