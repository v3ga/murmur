//--------------------------------------------------------------
// Variables à utiliser tout au cours du sketch
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
this.a = 0; //this.xAccroche+this.v*Math.cos(of.DegToRad(60));
this.b = 0;
this.c = 0;
this.d = 0;
this.e = 0;
this.f = 0;
this.angle = 0;

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup() {}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt) {
  println(this.volume);
  this.angle += dt * 360;

}

//--------------------------------------------------------------
// Appel pour le dessin sur la surface
//
// (w,h) sont les dimensions de la surface en pixels
//
// fonction de dessin (et autres)
// https://code.google.com/p/ofxjavascript/wiki/ofxJSGlobalFunctions
//
function draw(w, h) {
  of.SetColor(0,0,0,20);
  of.Rect(0,0,w,h);
  of.SetColor(255,255,255);
  of.SetLineWidth(1);
  of.NoFill(); 
  //of.Ellipse(this.xAccroche, this.yAccroche, 30, 30);
  //of.Line(0,0,w,h);
  //of.Triangle(10,20,30,40,50,60);

  this.v = w / 2 * this.volume;
  this.loop=7;
  for(i=0;i<this.loop;i++){
   this.a = this.xAccroche + this.v * Math.cos(of.DegToRad(this.angle+360/this.loop*(i)));
  this.b = this.yAccroche - this.v * Math.sin(of.DegToRad(this.angle+360/this.loop*(i)));
  this.c = this.xAccroche + this.v * Math.cos(of.DegToRad(this.angle+360/this.loop*(i+1)));
  this.d = this.yAccroche - this.v * Math.sin(of.DegToRad(this.angle+360/this.loop*(i+1)));
  of.Line(this.a, this.b, this.c, this.d);
  }
//  this.a = this.xAccroche + this.v * Math.cos(of.DegToRad(this.angle));
//  this.b = this.yAccroche - this.v * Math.sin(of.DegToRad(this.angle));
//  this.c = this.xAccroche + this.v * Math.cos(of.DegToRad(this.angle+120));
//  this.d = this.yAccroche - this.v * Math.sin(of.DegToRad(this.angle+120));
//  this.e = this.xAccroche + this.v * Math.cos(of.DegToRad(this.angle+240));
//  this.f = this.yAccroche - this.v * Math.sin(of.DegToRad(this.angle+240));
  //of.Bezier(this.xAccroche,this.yAccroche,this.a, this.b, this.c, this.d, this.e, this.f);


  //of.Line(this.xAccroche,this.yAccroche,0,this.volume*h);
}


//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId, volume, x, y) {
  this.volume = volume;
  this.xAccroche = x;
  this.yAccroche = y;
}