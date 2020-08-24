#ifndef MESH_HPP
#define MESH_HPP

#include "Object.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "gl.h"

namespace mode7
{

class Vertex
{
public:

    Vertex();
    ~Vertex();

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh : public Object
{
public:
    
    Mesh();
    virtual ~Mesh();

    void init();
    void setMaterial(Material&);
    void createFromBuffer(const float*, GLuint);
    void createFromArrays(std::vector<Vertex>&, std::vector<unsigned int>&);
    void createFromShape(int);
    void drawTriangles();
    virtual void draw(Shader&);

    void setAltShader(Shader&);

    Material material;

    static const int PLANE = 0;

protected:

    Shader* alt;

private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    bool elements;
    int triangles;
};

}

#endif /* MESH_HPP */