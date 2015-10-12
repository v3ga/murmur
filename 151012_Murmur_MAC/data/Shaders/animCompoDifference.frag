
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

in vec2 varyingtexcoord;

out vec4 outputColor;

void main(void)
{
	vec4 c1 = texture(tex1, varyingtexcoord);
	vec4 c2 = texture(tex2, varyingtexcoord);
	outputColor = vec4(BlendDifference(c1.rgb,c2.rgb), 1.0);
}


