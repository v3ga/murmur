/*
  ALB_tunnel_tests
 
 —
  Tests made for the tunnel animation on ALB performance at Les Victoires de la musique 2015
  https://www.youtube.com/watch?v=Ci3-rLQ2CJE
  https://vimeo.com/119611737
 
 —
 Developped and tested on : 
   - Processing 2.1.1 on MacOSX (10.10.2)
 
 —
 Julien @v3ga Gachadoat
 www.v3ga.net
 www.2roqs.com
 
*/

// ------------------------------------------------------------------------------------------------
PVector pointFar;
PVector dirLeft,dirRight;
float dirAngle = 2;
float dirSpeed = 15;
boolean doRotation = false;
float motifHeight = 30;

float period = 100; // milliseconds
float time0=0, time1 = 0;


ArrayList<Motif> motifsRight;
ArrayList<Motif> motifsLeft;

// ------------------------------------------------------------------------------------------------
void setup()
{
  size(800,600,P3D);
  pointFar = new PVector(0,0,-600);
  dirLeft = new PVector();
  dirRight = new PVector();
  motifsLeft = new ArrayList<Motif>();
  motifsRight = new ArrayList<Motif>();

  time0 = millis();
  time1 = period;
}

// ------------------------------------------------------------------------------------------------
void draw()
{
  // Update some vectors
  updateDir();

  // Timing
  time1 = (millis()-time0); 
  if (time1>=period)
  {
    releaseMotif();

    time1 = time0 = millis();
  }

  // Background
  background(0);

  // Transformations
  translate(width/2, height/2, 0);
  if (doRotation)
  {
    rotateZ(map(mouseX-width/2,-width/2,width/2,-PI,PI));
  }
  
  // Left
  Motif m;
  for (int i = motifsLeft.size()-1; i >= 0; i--) {
    m = motifsLeft.get(i);
    m.update();
    m.draw();
    if (m.pos.z>600)
      motifsLeft.remove(i);
  }

  // Right
  for (int i = motifsRight.size()-1; i >= 0; i--) {
    m = motifsRight.get(i);
    m.update();
    m.draw();
    if (m.pos.z>600)
      motifsRight.remove(i);
  }
}

// ------------------------------------------------------------------------------------------------
void mousePressed()
{
  releaseMotif();
}

// ------------------------------------------------------------------------------------------------
void updateDir()
{
  dirAngle = abs(map(mouseX-width/2,-width/2,width,-20,20));
  
  dirLeft.x = cos(radians(90+dirAngle));
  dirLeft.y = 0;
  dirLeft.z = sin(radians(90+dirAngle));
  dirLeft.normalize();
  dirRight.x = cos(radians(90-dirAngle));
  dirRight.y = 0;
  dirRight.z = sin(radians(90-dirAngle));
  dirRight.normalize();

  dirLeft.mult(dirSpeed);
  dirRight.mult(dirSpeed);
}

// ------------------------------------------------------------------------------------------------
void releaseMotif()
{
  if (random(1)<=0.5)  
    motifsLeft.add( new Motif(new PVector(0,0,-500), dirLeft,1) );
  else
    motifsRight.add( new Motif(new PVector(0,0,-500), dirRight,0) );
}

