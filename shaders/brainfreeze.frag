#version 130

uniform float level;
const vec2 x0 = vec2(400.,300.);
const vec2 iResolution = vec2(800., 600.);
const vec2 c = vec2(1.,0.);

void main()
{
    vec2 uv = gl_FragCoord.xy/iResolution.yy;
    float r0 = length(uv-x0/iResolution.yy)-level, scale = smoothstep(r0-.1, r0+.1, r0); 
    gl_FragColor += vec4(c.yyy, scale);
}
