#version 150

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect tex0;
uniform float amplitude;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;


void main()
{
	vec4 newPosition = position;
	float d = texture(tex0, texcoord).r;
	float angle = d*6.28;
	newPosition.z = position.z+d*amplitude;

	texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * newPosition;
}
