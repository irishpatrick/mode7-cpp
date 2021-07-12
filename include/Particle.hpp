#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "gl.hpp"

namespace mode7
{
    // forward declarations
    class Shader;

    class Particle
    {
    public:
        Particle();
        ~Particle();

        void init(Shader*, uint32_t, glm::vec4);
        void initGeometry();
        void setShader(Shader*);

        inline bool isVisible()
        {
            return m_life > 0.0;
        }

        inline void spawn(glm::vec3 pos, glm::vec3 vel, float life)
        {
            m_pos = pos;
            m_vel = vel;
            m_life = life;
            m_maxLife = life;
        }

        inline void setRate(float rate)
        {
            m_rate = rate;
        }

        inline void setColor(glm::vec4 rgba)
        {
            m_color = rgba;
        }

        bool update();
        void drawTriangles();
        void draw();

    private:
        glm::vec3 m_pos;
        glm::vec3 m_vel;
        glm::vec4 m_color;

        float m_life;
        float m_maxLife;
        float m_rate;

        Shader* m_shader;
        GLuint m_posShaderLoc;
        GLuint m_velShaderLoc;
        GLuint m_colorShaderLoc;
        GLuint m_timeShaderLoc;
        GLuint m_texShaderLoc;

        GLuint m_textureId;

        GLuint m_vao;

        friend class ParticleEmitter;
    };
} // namespace mode7


#endif /* PARTICLE_HPP */