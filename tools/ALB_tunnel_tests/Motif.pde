class Motif
{
  PVector pos;
  PVector dir;
  PVector[] points;
  
  
  Motif(PVector pos_, PVector dir_, int left)
  {
    pos = new PVector(pos_.x,pos_.y,pos_.z);
    dir = new PVector(dir_.x,dir_.y,dir_.z);
  
    points = new PVector[8];
    for (int i=0;i<8;i++)
      points[i] = new PVector();
    if (left>0)
    {
      points[0].set(0,-motifHeight,0);
      points[1].set(-motifHeight,-5,0);
      points[2].set(-motifHeight,5,0);
      points[3].set(0,motifHeight,0);

      points[4].set(0,motifHeight,10);
      points[5].set(-motifHeight,5,10);
      points[6].set(-motifHeight,-5,10);
      points[7].set(0,-motifHeight,10);
    }
    else
    {
      points[0].set(0,-20,0);
      points[1].set(20,-5,0);
      points[2].set(20,5,0);
      points[3].set(0,20,0);

      points[4].set(0,20,10);
      points[5].set(20,5,10);
      points[6].set(20,-5,10);
      points[7].set(0,-20,10);
    
    }
  }
  
  void update()
  {
    pos.add(dir);
  }
  
  void draw()
  {
    pushMatrix();
    translate(pos.x,pos.y,pos.z);
    pushStyle();
    stroke(#FFFFFF, map(pos.z,-500,500,0,255));
    strokeWeight(3);
    beginShape();
    for (int i=0;i<8;i++)
      vertex(points[i].x,points[i].y,points[i].z);
    endShape(CLOSE);
    popStyle();
    popMatrix();
  }
};
