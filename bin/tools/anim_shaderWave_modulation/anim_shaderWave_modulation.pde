void setup()
{
  size(500,500);
}

void draw()
{
  background(200);
  translate(width/2,height/2);
  noFill();
  beginShape();
  float step = 0.5f;
  float R = 200;
  
  for (float a=0.0f; a < 360.0; a = a+step)
  {
    float r = R + 30*sin(10*radians(a+millis()/100.0))*cos( 3*radians(a+millis()/30) );
    vertex( r*cos(radians(a)), r*sin(radians(a)) );
  }
  endShape(CLOSE);
}