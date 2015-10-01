#version 150

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

in vec2 varyingtexcoord;

out vec4 outputColor;

void main(void)
{
	vec4 c1 = texture(tex1, varyingtexcoord);
	vec4 c2 = texture(tex2, varyingtexcoord);
   	outputColor = c1+c2;
}