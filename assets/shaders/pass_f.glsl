#version 330 core

out vec4 color;
in vec2 tc;
uniform sampler2D colorTex;

void main()
{
    color = texture(colorTex, tc);
}