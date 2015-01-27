#version 150

uniform vec2 resolution;
uniform vec2 anchor;
uniform float texSize;
uniform sampler2DRect texWave;
uniform float nbWaves;
uniform float intensityWave;

out vec4 outputColor;

void main(void)
{
   float d = distance(gl_FragCoord.xy, anchor)/resolution.x * texSize;
   vec3 c = intensityWave*texture(texWave, vec2(d,0.5)).xyz;
   outputColor = vec4(c,1.0);
}