#version 150

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;

void main
{
	texCoordVarying = texcoord;
	gl_Position = position;
}

/*
#version 120
#extension GL_ARB_texture_rectangle : enable

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_Vertex;
}
*/
