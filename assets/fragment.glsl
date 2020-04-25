#version 330 core

in vec2 uv;
out vec4 color;

uniform sampler2D tex;

vec4 blur(sampler2D image, vec2 uv, vec2 resolution, vec2 direction)
{
    vec4 color = vec4(0.1, 0.1, 0.2, 1.0);
    vec2 off1 = vec2(1.411764705882353) * direction;
    vec2 off2 = vec2(3.2941176470588234) * direction;
    vec2 off3 = vec2(5.176470588235294) * direction;

    color += texture2D(image, uv) * 0.1964825501511404;
    color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
    color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
    color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
    color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
    color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
    color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
    return color;
}

void main()
{
    vec4 tc = texture(tex, uv);
    vec4 far = blur(tex, uv, vec2(1920.0, 1080.0), vec2(10.0, 10.0));
    color = mix(tc, far, smoothstep(0.1, 100.0, gl_FragCoord.z / gl_FragCoord.w));
    //color = texture(tex, uv);
}