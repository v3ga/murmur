#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 varyingtexcoord;

void main()
{
	varyingtexcoord = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}
