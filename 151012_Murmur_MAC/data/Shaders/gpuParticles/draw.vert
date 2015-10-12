#version 150

uniform sampler2DRect particles0;
uniform sampler2DRect particles1;

in vec4 position;
in vec2 texcoord;


void main()
{
	texCoordVarying = texcoord;

	gl_Position = modelViewProjectionMatrix * vec4(texture(particles0, texcoord.st).xyz, 1.0);
}


/*

#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect particles0;
uniform sampler2DRect particles1;

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(texture2DRect(particles0, gl_TexCoord[0].st).xyz, 1.0);
}
*/

