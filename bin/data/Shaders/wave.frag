uniform vec2 resolution;
uniform vec2 anchor;
uniform float texSize;
uniform sampler2DRect texWave;

void main(void)
{
    float d = distance(gl_FragCoord.xy, anchor)/resolution.x * texSize;
    vec3 c = texture2DRect(texWave, vec2(d,0.5)).xyz;
    
    gl_FragColor = vec4(c,1.0);
}