#version 130

uniform float iTime;
const vec2 iResolution = vec2(800., 600.);
const vec2 c = vec2(1.,0.);

int main()
{
    vec2 uv = gl_FragCoord.xy/iResolution.yy;
    float r0 = length(x-)-r, scale = smoothstep(r0-.1, r0+.1, length(x-uv)); 
    gl_FragColor = vec4(c.yyy, scale);
}
