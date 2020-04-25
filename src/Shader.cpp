#include "Shader.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#define MAT(x) &(x[0][0])
#define VEC(x) &(x[0])

std::string openFile(const std::string& fn)
{
    std::ifstream fp(fn);
    if (!fp)
    {
        return "";
    }

    std::stringstream code;
    code << fp.rdbuf();
    fp.close();

    return code.str();
}

int compile(GLuint id, const std::string& code)
{
    GLint res = 0;
    int loglen;

    const char* ptr = code.c_str();
    glShaderSource(id, 1, &ptr, nullptr);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0)
    {
        std::vector<char> errMsg;
        errMsg.reserve(loglen + 1);
        glGetShaderInfoLog(id, loglen, NULL, &errMsg[0]);
        printf("%s\n", &errMsg[0]);
    }

    return res;
}

Shader::Shader() :
    id(0),
    projection(0),
    view(0),
    model(0),
    diffuseTexture(0),
    uv_tile(0)
{
}

Shader::~Shader()
{
}

int Shader::open(const std::string& vfn, const std::string& ffn)
{
    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vs_code = openFile(vfn);
    std::string fs_code = openFile(ffn);
    if (!vs_code.size() || !fs_code.size())
    {
        return -1;
    }

    std::cout << "Shader: compile " << vfn << std::endl;
    compile(vsid, vs_code);
    ec();
    std::cout << "Shader: compile " << ffn << std::endl;
    compile(fsid, fs_code);
    ec();

    std::cout << "Shader: link" << std::endl;
    GLuint pid = glCreateProgram();
    glAttachShader(pid, vsid);
    glAttachShader(pid, fsid);
    glLinkProgram(pid);
    ec();

    GLint res;
    int loglen;

    glGetProgramiv(pid, GL_LINK_STATUS, &res);
    glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0)
    {
        std::vector<char> errMsg;
        errMsg.reserve(loglen + 1);
        glGetShaderInfoLog(id, loglen, NULL, &errMsg[0]);
        printf("%s\n", &errMsg[0]);
    }

    glDetachShader(pid, vsid);
    glDetachShader(pid, fsid);
    glDeleteShader(vsid);
    glDeleteShader(fsid);

    id = pid;
    glUseProgram(id);
    projection = glGetUniformLocation(id, "p");
    view = glGetUniformLocation(id, "v");
    model = glGetUniformLocation(id, "m");
    diffuseTexture = glGetUniformLocation(id, "tex");
    uv_tile = glGetUniformLocation(id, "uv_tile");

    ec();

    return 0;
}

void Shader::use()
{
    glUseProgram(id);
    //ec();

    glUniformMatrix4fv(projection, 1, GL_FALSE, MAT(Camera::getProjection()));
    glUniformMatrix4fv(view, 1, GL_FALSE, MAT(Camera::getView()));
    //ec();
}

void Shader::setMaterial(Material& m)
{
    glUniform1ui(diffuseTexture, m.getDiffuseTexture());
    glUniform1i(uv_tile, m.tile);
}

void Shader::setModel(Object& o)
{
    glUniformMatrix4fv(model, 1, GL_FALSE, MAT(o.getWorldMatrix()));
}
