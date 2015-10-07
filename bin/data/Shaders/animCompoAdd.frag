#version 150

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

in vec2 varyingtexcoord;

out vec4 outputColor;

void main(void)
{
	vec4 c1 = texture(tex1, varyingtexcoord);
	vec4 c2 = texture(tex2, varyingtexcoord);
//   	outputColor = max(c1.rgb-c2.rgb, vec3(0.0,0.0,0.0));
	vec3 add = min(c2.xyz+c1.xyz,vec3(1.0));
	outputColor = vec4(add,1.0);
}