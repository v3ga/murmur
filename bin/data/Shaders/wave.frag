uniform vec2 resolution;
uniform vec2 anchor;
uniform float texSize;
uniform sampler2DRect texWave;
uniform float nbWaves;
uniform float intensityWave;

void main(void)
{
    float d = distance(gl_FragCoord.xy, anchor)/resolution.x * texSize;
    vec3 c = intensityWave*texture2DRect(texWave, vec2(d,0.5)).xyz;
    
//    gl_FragColor = gl_FragColor + vec4(c,0.90);
   gl_FragColor = vec4(c,1.0);
 //   gl_FragColor = mix(gl_FragColor,vec4(c,1.0),intensityWave);
}