#version 330 core

precision highp float;

layout(location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 tc;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    gl_Position = m * (vec4(position, 1));
    tc = uv;
}
