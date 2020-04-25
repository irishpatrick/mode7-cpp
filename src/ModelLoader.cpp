#include "ModelLoader.hpp"
#include "Texture.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

typedef enum _line_t {MTLLIB, O, V, VT, VN, F, NONE} line_t;

Mesh ModelLoader::open(const std::string& fn)
{
    Mesh mesh;
    Material mat;

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return mesh;
    }

    line_t mode = NONE;
    int left = 0;
    std::string line;
    std::vector<std::string> parts;
    std::vector<std::string> subPart;
    while(std::getline(in, line))
    {
        if (line[0] == '#')
        {
            continue;
        }

        boost::algorithm::split(parts, line, boost::algorithm::is_space());
        for (auto& part : parts)
        {
            if (mode == NONE)
            {
                if (part == "mtllib")
                {
                    mode = MTLLIB;
                    left = 1;
                }
                else if (part == "v")
                {
                    mode = V;
                    left = 3;
                }
                else if (part == "vt")
                {
                    mode = VT;
                    left = 2;
                }
                else if (part == "vn")
                {
                    mode = VN;
                    left = 3;
                }
                else if (part == "f")
                {
                    mode = F;
                    left = 3;
                }
            }
            else
            {
                --left;

                if (mode == MTLLIB)
                {
                    mat = openMaterial(part);
                }
                else if (mode == V)
                {
                    vertices.push_back((float)std::atof(part.c_str()));
                }
                else if (mode == VT)
                {
                    uvs.push_back((float)std::atof(part.c_str()));
                }
                else if (mode == VN)
                {
                    normals.push_back((float)std::atof(part.c_str()));
                }
                else if (mode == F)
                {
                    boost::algorithm::split(subPart, part, boost::algorithm::is_any_of("/"));
                    for (auto& sp : subPart)
                    {
                        indices.push_back((unsigned int)std::atoi(sp.c_str()));
                    }
                }
            }

            if (left == 0)
            {
                mode = NONE;
            }
        }
    }

    std::vector<float> buffer;
    buffer.resize(8 * indices.size() / 3);

    for (int i = 0; i < indices.size() / 3; ++i)
    {
        int vi = indices[i * 3 + 0] - 1;
        int ui = indices[i * 3 + 1] - 1;
        int ni = indices[i * 3 + 2] - 1;

        buffer[i * 8 + 0] = vertices[vi * 3 + 0];
        buffer[i * 8 + 1] = vertices[vi * 3 + 1];
        buffer[i * 8 + 2] = vertices[vi * 3 + 2];

        buffer[i * 8 + 3] = normals[ni * 3 + 0];
        buffer[i * 8 + 4] = normals[ni * 3 + 1];
        buffer[i * 8 + 5] = normals[ni * 3 + 2];

        buffer[i * 8 + 6] = uvs[ui * 2 + 0];
        buffer[i * 8 + 7] = -uvs[ui * 2 + 1];
    }

    mesh.createFromBuffer(&buffer[0], buffer.size());
    mesh.setMaterial(mat);

    return mesh;
}

Material ModelLoader::openMaterial(const std::string& fn)
{
    Material mat;

    std::ifstream in(fn);
    if (!in)
    {
        std::cout << "cannot open " << fn << std::endl;
        return mat;
    }

    bool map = false;
    std::string line;
    std::vector<std::string> parts;
    while (std::getline(in, line))
    {
        if (line[0] == '#')
        {
            continue;
        }

        boost::algorithm::split(parts, line, boost::algorithm::is_space());
        for (auto& part : parts)
        {
            if (!map)
            {
                if (part == "map_Kd")
                {
                    map = true;
                }
            }
            else
            {
                mat.setDiffuseTexture(Texture::open(part));
                map = false;
            }
            
        }
    }

    return mat;
}