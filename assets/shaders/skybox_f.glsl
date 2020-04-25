#version 330 core

in vec2 uv;
out vec4 color;

uniform sampler2D tex;

void main()
{
    vec4 tc = texture(tex, uv);
    if (tc.a < 0.5)
    {
        discard;
    }
    color = tc;
    //color = vec4(1, 0, 1, 1);
}