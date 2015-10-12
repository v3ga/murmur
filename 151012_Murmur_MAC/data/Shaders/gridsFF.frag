#version 150

uniform vec2 resolution;
uniform sampler2DRect tex0;
uniform int colorFromDevice;
uniform float luminance;

in vec2 texCoordVarying;

out vec4 outputColor;

void main(void)
{
	if (colorFromDevice>0)
	   outputColor = vec4(luminance,luminance,luminance,0.0)+texture(tex0, texCoordVarying);
	else
   		outputColor = vec4(1.0,1.0,1.0,1.0);
}