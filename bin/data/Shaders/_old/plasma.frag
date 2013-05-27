uniform float resolutionx;
uniform float resolutiony;
uniform float time;
uniform float fSin2;
uniform float dSin2;

void main(void)
{
	float freqSin2 = fSin2; // 0.2 et 4
	float divSin2 = dSin2;

   float x = gl_FragCoord.x;
   float y = gl_FragCoord.y;
   float mov0 = x+y+cos(sin(time)*2.)*100.+sin(x/100.)*1000.;
   float mov1 = y / resolutiony / 0.2 + time;
   float mov2 = x / resolutionx / 0.2;
   float c1 = abs(sin(mov1+time)/2.+mov2/2.-mov1-mov2+time);
   float c2 = abs(sin(c1+/*sin(mov0/100.+time)*/+sin(y/40.+time)+sin((x+y)/divSin2)*freqSin2));
   float c3 = abs(sin(c2+cos(mov1+mov2+c2)+cos(mov2)+sin(x/1000.)));
   gl_FragColor = vec4( 1.0-c2,1.0-c2,1.0-c2,1.0);
}