#version 330 core

in float depth;
out vec4 color;

void main()
{
    float d = depth / 1500;
    color = vec4(d, d, d, 1);
}
