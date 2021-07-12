#include "Particle.hpp"
#include "Shader.hpp"

#include <cassert>

static const GLfloat plane_vertex_data[] = {
   -1.0f, -1.0f,  0.0f,      0.0f,  0.0f,
    1.0f, -1.0f,  0.0f,      1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,      1.0f, -1.0f,
   -1.0f,  1.0f,  0.0f,      0.0f, -1.0f,
};

static const GLuint plane_index_data[] = {
    0, 1, 2,
    2, 3, 0,
};

namespace mode7
{
    Particle::Particle() :
        m_shader(nullptr),
        m_life(0.0),
        m_rate(0.1),
        m_textureId(0),
        m_vao(0),
        m_vel(0, 0.1, 0)
    {

    }

    Particle::~Particle()
    {

    }

    void Particle::init(Shader* shdr, uint32_t tex, glm::vec4 color)
    {
        m_textureId = tex;
        m_color = color;
        initGeometry();
        setShader(shdr);
    }

    void Particle::initGeometry()
    {
        GLuint vbo;
        GLuint ebo;

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(m_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertex_data), plane_vertex_data, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_index_data), plane_index_data, GL_STATIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 3,
            GL_FLOAT, GL_FALSE,
            5 * sizeof(float),
            (void*)0
        );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 2,
            GL_FLOAT, GL_FALSE,
            5 * sizeof(float),
            (void*)(3 * sizeof(float))
        );

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Particle::setShader(Shader* sp)
    {
        m_shader = sp;
        assert(m_shader != nullptr);

        m_shader->cacheCameraMatrices();
        m_posShaderLoc = glGetUniformLocation(m_shader->pid(), "pos");
        m_velShaderLoc = glGetUniformLocation(m_shader->pid(), "vel");
        m_colorShaderLoc = glGetUniformLocation(m_shader->pid(), "color");
        m_timeShaderLoc = glGetUniformLocation(m_shader->pid(), "t");
        m_texShaderLoc = glGetUniformLocation(m_shader->pid(), "sprite");
    }

    bool Particle::update()
    {
        bool status = false;

        m_life -= m_rate;
        if (m_life <= 0)
        {
            status = true;
        }

        return status;
    }

    void Particle::drawTriangles()
    {
        assert(m_vao != 0);
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Particle::draw()
    {
        m_shader->use();
        
        // position, motion data
        glUniform3fv(m_posShaderLoc, 1, VEC(m_pos));
        glUniform3fv(m_velShaderLoc, 1, VEC(m_vel));
        glUniform4fv(m_colorShaderLoc, 1, VEC(m_color));
        glUniform1f(m_timeShaderLoc, 1.0 - m_life / m_maxLife);

        // texture data
        glUniform1i(m_texShaderLoc, 0);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        drawTriangles();
    }
} // namespace mode7
