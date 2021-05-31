#ifndef GL_HPP
#define GL_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#ifdef _WIN32
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <iostream>

inline void gl_err_check()
{
    GLuint err = glGetError();
    if (err)
    {
        std::cout << gluErrorString(err) << std::endl;
    }
}

// TODO add debug check
#define ec gl_err_check()

#endif /* GL_HPP */
