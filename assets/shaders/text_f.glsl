#version 330 core

out vec4 color;
in vec2 tc;
uniform sampler2D tex;

void main()
{
    color = texture(tex, tc);
    if (color.a < 0.5)
    {
        discard;
    }
    //color = vec4(1.0, 0.0, 1.0, 1.0);
}
