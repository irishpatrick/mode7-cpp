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

        void draw(Shader*);

    private:
        glm::vec3 m_pos;
        glm::vec3 m_vel;
        glm::vec4 m_color;
        float m_life;
        bool m_visible;

        friend class ParticleEmitter;
    };
} // namespace mode7


#endif /* PARTICLE_HPP */