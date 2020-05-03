#version 330 core

in vec2 uv;
out vec4 color;

uniform sampler2D diffuseMap[16];

void main()
{
    vec4 tc = texture(diffuseMap[0], uv);
    if (tc.a < 0.3)
    {
        discard;
    }
    color = tc;
    //color = vec4(0.2, 0.2, 0.2, 1);
}