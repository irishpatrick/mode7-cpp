#version 330 core

in vec2 uv;
out vec4 color;

uniform sampler2D diffuseMap_0;

void main()
{
    vec4 tc = texture(diffuseMap_0, uv);
    if (tc.a < 0.3)
    {
        discard;
    }
    color = tc;
    //color = vec4(0.2, 0.2, 0.2, 1);
}