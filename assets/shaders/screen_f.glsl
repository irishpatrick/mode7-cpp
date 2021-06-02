#version 330 core

out vec4 color;
in vec2 tc;
uniform sampler2D depthTex;
uniform sampler2D colorTex;

void main()
{
    float pi_2 = 3.14159264338327 * 2.0;
    float blur_n_dir = 16.0;
    float blur_qual = 3.0;
    float size = 4.0;
    float depth = length(texture(depthTex, tc).xyz);
    vec2 rad = vec2(size / 1280, size / 720);

    color = texture(colorTex, tc);

    //if (depth > 1.0)
    //if (gl_FragCoord.y > 720 / 2)
    {
        for (float d = 0.0; d < pi_2; d += pi_2 / blur_n_dir)
        {
            for (float i = 1.0 / blur_qual; i <= 1.0; i += 1.0 / blur_qual)
            {
                color += vec4(texture(colorTex, tc + depth * vec2(cos(d), sin(d)) * rad * i).xyz, color.w);
            }
        }
        color /= blur_qual * blur_n_dir - 15.0;
    }
}