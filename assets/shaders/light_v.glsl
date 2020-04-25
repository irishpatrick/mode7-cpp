#version 330 core

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertex_uv;

out vec3 eye;
out vec3 norm;
out vec3 pos;
out vec2 uv;
out vec3 fragpos;
out mat4 vm;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform vec3 velocity;
uniform float ex;

void main()
{
    //vec3 evel = ex * velocity;
    //gl_Position = p * v * m * (vec4(position, 1) + vec4(evel, 1));
    
    gl_Position = p * v * m * (vec4(position, 1));

    uv = vertex_uv;

    mat4 nm = transpose(inverse(m));
    norm = normalize(vec3(nm * vec4(normal, 1.0)));
    fragpos = vec3(m * vec4(pos, 1.0));
}