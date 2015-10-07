import controlP5.*;

// ------------------------------------------------------
float degrade = 0.2; 
color black = color(0);
color white = color(255);

ControlP5 controls;

// ------------------------------------------------------
void setup()
{
  size(512, 512, P2D);
  controls = new ControlP5(this);
  noStroke();

  controls.begin(10, 10);
//  controls.addToggle("left").setValue(drawLeft).plugTo(drawLeft).linebreak();
//  controls.addToggle("right").setValue(drawRight);
  controls.end();
}

// ------------------------------------------------------
boolean once = false;
void draw()
{
  background(0);
  if (!once)
  {
    drawCircle(width/2,height/2,width/2,50);
    once = false;
  }
}

// ------------------------------------------------------
void drawCircle(float x, float y, float r, int nb)
{
  
  beginShape(TRIANGLE_FAN);
  fill(white);
  vertex(x,y);
  float xx,yy,angle;
  fill(black);
  for (int i=0;i<=nb;i++)
  {
    angle = float(i) / float(nb) * TWO_PI;
    xx = x+r*cos(angle);
    yy = y+r*sin(angle);
    vertex(xx,yy);
  }
  
  endShape();
}

// ------------------------------------------------------
void keyPressed()
{
  if (key == ' ')
    saveFrame("degrade_radial_"+degrade+".png");
}

// ------------------------------------------------------
void controlEvent(ControlEvent theEvent) 
{
  String name = theEvent.name();
}

