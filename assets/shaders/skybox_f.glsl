#version 330 core

in vec2 uv;
out vec4 color;

uniform sampler2D tex;

void main()
{
    vec4 tc = texture(tex, uv);
    if (tc.a < 0.3)
    {
        discard;
    }
    color = tc;
    //color = vec4(0.2, 0.2, 0.2, 1);
}