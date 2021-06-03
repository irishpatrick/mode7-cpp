#version 330 core

in vec2 uv;
in float depth;
out vec4 color;

uniform sampler2D diffuseMap[16];

vec4 blurf(sampler2D image, vec2 uv, vec2 resolution, vec2 direction)
{
    vec4 color = vec4(0.05, 0.05, 0.08, 0.0);
    vec2 off1 = vec2(1.411764705882353) * direction;
    vec2 off2 = vec2(3.2941176470588234) * direction;
    vec2 off3 = vec2(5.176470588235294) * direction;

    color += vec4(0.0, 0.0, 0.0, texture2D(image, uv).w);
    color += vec4(texture2D(image, uv).rgb * 0.1964825501511404, 0.0);
    color += vec4(texture2D(image, uv + (off1 / resolution)).rgb * 0.2969069646728344, 0.0);
    color += vec4(texture2D(image, uv - (off1 / resolution)).rgb * 0.2969069646728344, 0.0);
    color += vec4(texture2D(image, uv + (off2 / resolution)).rgb * 0.09447039785044732, 0.0);
    color += vec4(texture2D(image, uv - (off2 / resolution)).rgb * 0.09447039785044732, 0.0);
    color += vec4(texture2D(image, uv + (off3 / resolution)).rgb * 0.010381362401148057, 0.0);
    color += vec4(texture2D(image, uv - (off3 / resolution)).rgb * 0.010381362401148057, 0.0);
    return color;
}

void main()
{
    vec4 tc = texture(diffuseMap[0], uv);
    //vec4 far = blurf(tex, uv, vec2(15360.0, 8640.0), vec2(1.5 * (gl_FragCoord.z / gl_FragCoord.w), 0.5));
    //vec4 texel = mix(tc, far, smoothstep(0.1, 100.0, gl_FragCoord.z / gl_FragCoord.w));
    vec4 texel = tc;
    if (texel.a < 0.3)
    {
        discard;
    }
    
    float fogNear = 3;
    float fogFar = 100;
    float fogDepth = (depth - fogNear) / 1500;
   
    //color = texel;
    //color = mix(texel, 0.7 * vec4(0.8, 0.9, 1.0, 1.0), smoothstep(fogNear, fogFar, (fogDepth)));
    color = mix(texel, 0.3 * texel + 0.7 * vec4(0.8, 0.9, 1.0, 1.0), sqrt(fogDepth));
    //color = vec4(gl_FragCoord.x / 1280, gl_FragCoord.y / 720, gl_FragCoord.z / 1000, 1);
    //color = vec4(clr, 1);
}
