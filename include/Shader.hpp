#ifndef SHADER_HPP
#define SHADER_HPP

#include "gl.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include <string>
#include <vector>

#define MAT(x) &(x[0][0])
#define VEC(x) &(x[0])

namespace mode7
{

class Shader
{
public:

    Shader();
    ~Shader();

    int open(const std::string&, const std::string&);
    void cacheCameraMatrices();
    void cacheLocations();

    void use();
    void onlyUse();
    void setMaterial(Material&);
    void setModel(Object&);

    inline GLuint pid()
    {
        return id;
    }

private:

    GLuint id;
    GLuint projection;
    GLuint view;
    GLuint model;
    GLuint diffuseTexture;
    GLuint uv_tile;

    std::vector<GLuint> m_diffuseMaps;
    std::vector<GLuint> m_specularMaps;
};

}

#endif /* SHADER_HPP */