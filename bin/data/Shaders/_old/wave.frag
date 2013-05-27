uniform float resolutionx;
uniform float resolutiony;
uniform float centerx;
uniform float centery;
uniform float time;
uniform float frequence;
uniform float timeFactor;

void main(void)
{
   float x = gl_FragCoord.x;
   float y = gl_FragCoord.y;

	float freq = frequence;
	float range = freq*3.14*2.0;
	
	
	vec2 center = vec2(centerx,centery);
	vec2 p = vec2(x,y);
	float d = distance(center,p)/center.y;
	
	float c = 0.5*(1.0+cos(-timeFactor*time+d*range));

/*
   float mov0 = x+y+cos(sin(time)*2.)*100.+sin(x/100.)*1000.;
   float mov1 = y / resolutiony / 0.2 + time;
   float mov2 = x / resolutionx / 0.2;
   float c1 = abs(sin(mov1+time)/2.+mov2/2.-mov1-mov2+time);
   float c2 = abs(sin(c1+sin(mov0/100.+time)+sin(y/40.+time)+sin((x+y)/100.)*4.));
   float c3 = abs(sin(c2+cos(mov1+mov2+c2)+cos(mov2)+sin(x/1000.)));
*/

   gl_FragColor = vec4(c,c,c,1.0);
}