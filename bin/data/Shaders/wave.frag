#version 150

uniform vec2 resolution;
uniform vec2 anchor;
uniform float texSize;
uniform sampler2DRect texWave;
uniform float intensityWave;
uniform float ampSine;
uniform float freqSine;
uniform float freqCosine;
uniform float time;


out vec4 outputColor;

void main(void)
{
   float a = atan(gl_FragCoord.y - anchor.y, gl_FragCoord.x - anchor.x);
   float d = (distance(gl_FragCoord.xy, anchor)+ampSine*sin(freqSine*a)*cos(freqCosine*(a+time*1.35)))/resolution.x * texSize;
   vec3 c = 1.0*texture(texWave, vec2(d,0.5)).xyz;
   outputColor = vec4(c,1.0);
}
