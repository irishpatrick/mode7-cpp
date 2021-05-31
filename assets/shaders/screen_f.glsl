#version 330 core

out vec4 color;
in vec2 tc;
uniform sampler2D colorTex;
uniform sampler2D depthTex;

void main()
{
    color = texture(colorTex, tc);
    //color = vec4(1.0, 0.0, 1.0, 1.0);
}