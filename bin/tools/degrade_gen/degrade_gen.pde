import controlP5.*;

// ------------------------------------------------------
float degrade = 0.2; 
color black = color(0);
color white = color(255);
boolean drawLeft = true;
boolean drawRight = true;
float x0, w0, x1, w1, x2, w2;

ControlP5 controls;

// ------------------------------------------------------
void setup()
{
  size(800, 600, P2D);
  controls = new ControlP5(this);
  noStroke();

  controls.begin(10, 10);
  controls.addToggle("left").setValue(drawLeft).plugTo(drawLeft).linebreak();
  controls.addToggle("right").setValue(drawRight);
  controls.end();
}

// ------------------------------------------------------
void draw()
{
  degrade = map(mouseX, 0, width, 0.0, 0.5);

  background(155);

  // Col 0 
  if (drawLeft)
  {
    x0 = 0;
    w0 = degrade * (float)width;
    drawQuad(x0, 0, w0, height, black, white);
  }
  else {
    x0 = 0;
    w0 = 0;
  }

  // Col 1 
  x1 = x0+w0;
  if (drawRight){
    w1 = (float)width - (w0+(float)width*degrade);
  }else{
    w1 = (float)width - w0;
  }
  drawQuad(x1, 0, w1, height, white, white);

  // Col 2 
  if (drawRight)
  {
    x2 = x1 + w1;
    w2 = degrade * (float)width;
    drawQuad(x2, 0, w2, height, white, black);
  }
}

// ------------------------------------------------------
void drawQuad(float x, float y, float w, float h, color colorLeft, color colorRight)
{
  beginShape(QUADS);
  fill(colorLeft);
  vertex(x, y);
  fill(colorRight);
  vertex(x+w, y);
  fill(colorRight);
  vertex(x+w, y+h);
  fill(colorLeft);
  vertex(x, y+h);
  endShape();
}

// ------------------------------------------------------
void keyPressed()
{
  if (key == ' ')
    saveFrame("degrade_"+degrade+".png");
}

// ------------------------------------------------------
void controlEvent(ControlEvent theEvent) 
{
  String name = theEvent.name();
  if (name.equals("left")) drawLeft = theEvent.getValue() > 0.0f;
  if (name.equals("right")) drawRight = theEvent.getValue() > 0.0f;
}

