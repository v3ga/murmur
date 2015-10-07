#version 150

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect tex0;
//uniform sampler2DRect tex1;
uniform float amplitude;
//uniform vec2 devicePos;
uniform vec2 devicePos[10];
uniform int devicePosNb;
uniform float torsion;
uniform float torsionRadius;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;


void main()
{
	vec4 newPosition = position;
	float d = texture(tex0, texcoord).r;
	float angle = d*6.28;
	
	vec2 dist;
	float rot=0.0;
	for (int i=0; i<devicePosNb; i++)
	{
		dist = newPosition.xy - vec2(devicePos[i].x,devicePos[i].y);
		rot = torsion*max( 0.0, 1.0-length(dist)/torsionRadius );
		vec2 r = vec2(
			dist.x*cos(rot) - dist.y * sin(rot),
			dist.x*sin(rot) + dist.y * cos(rot)
		) - dist;

		// Rotation
		newPosition.x += r.x;
		newPosition.y += r.y;
	}

	
	// Translate
	newPosition.z = position.z+d*amplitude;
	
	// Pass to fragment
	texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * newPosition;
}
