#version 330 core

precision lowp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertex_uv;

out vec2 uv;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform vec3 velocity;
uniform float ex;

void main()
{
    vec3 evel = ex * velocity;
    //gl_Position = p * v * m * (vec4(position, 1) + vec4(evel, 1));
    gl_Position = p * v * m * (vec4(position, 1));

    uv = vertex_uv;
}