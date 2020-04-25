#version 330 core

#define MAX_POINT_LIGHTS 100

// structures
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuse;
};

// shared vars
in mat4 vm;
in vec3 eye;
in vec3 norm;
in vec3 pos;
in vec3 fragpos;

in vec2 uv;
out vec4 color;

uniform sampler2D tex;

uniform DirectionalLight directional_light;

uniform int num_point_lights;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

void main()
{
    color = vec4(0.0);
    vec4 tc = texture(tex, uv);
    if (tc.a < 0.5)
    {
        discard;
    }

    vec3 ldir = normalize(directional_light.direction - fragpos);
    ldir = directional_light.direction;
    float diff = max(dot(norm, ldir), 0.1);

    color += diff * vec4(directional_light.diffuse, 1.0) * tc;
}