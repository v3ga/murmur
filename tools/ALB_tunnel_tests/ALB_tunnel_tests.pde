PVector pointFar;
PVector dirLeft,dirRight;
float dirAngle = 2;
float dirSpeed = 5;
boolean doRotation = false;
float motifHeight = 30;


ArrayList<Motif> motifsRight;
ArrayList<Motif> motifsLeft;

void setup()
{
  size(800,600,P3D);
  pointFar = new PVector(0,0,-600);
  dirLeft = new PVector();
  dirRight = new PVector();
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


  motifsLeft = new ArrayList<Motif>();
  motifsRight = new ArrayList<Motif>();
}

void draw()
{
  background(0);
  translate(width/2, height/2, 0);
  if (doRotation)
  {
    rotateZ(map(mouseX-width/2,-width/2,width/2,-PI,PI));
  }
  Motif m;
  for (int i = motifsLeft.size()-1; i >= 0; i--) {
    m = motifsLeft.get(i);
    m.update();
    m.draw();
    if (m.pos.z>600)
      motifsLeft.remove(i);
  }
  for (int i = motifsRight.size()-1; i >= 0; i--) {
    m = motifsRight.get(i);
    m.update();
    m.draw();
    if (m.pos.z>600)
      motifsRight.remove(i);
  }
}

void mousePressed()
{
  if (random(1)<=0.5)  
    motifsLeft.add( new Motif(new PVector(0,0,-500), dirLeft,1) );
  else
    motifsRight.add( new Motif(new PVector(0,0,-500), dirRight,0) );
}

