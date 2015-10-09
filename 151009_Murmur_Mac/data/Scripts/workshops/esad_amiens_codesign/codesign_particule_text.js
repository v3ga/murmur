//--------------------------------------------------------------
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
this.particules = [];
this.angle = 0;
this.pos = 0;
this.font = null;
  this.text = ["twitter", "google", "youtube", "linkedin","yahoo","error","data","facebook","lolcat","cheval","thieulloy l\'abbaye","processing","codesignesad","bdx","amiens","paris"];


//--------------------------------------------------------------

function Particule(life, font, pos, text) {
  this.pos = pos;
  this.text = text;
  this.font = font;
  this.x = 0;
  this.y = 0;
  this.life = life;
  this.vx = of.Random(-30, 30);
  this.vy = of.Random(-30, 30);
  this.alpha = 0;
  this.opacity = of.Random(0, 100);

  this.update = function (dt) {
    this.x += this.vx * dt;
    this.y += this.vy * dt;
    this.alpha = dt * 36;
    println("alpha = "+this.alpha)
    this.life--;
  }
  this.draw = function () {
    of.SetColor(255, 255, 255, this.opacity);
    if (this.font) {
      of.Rotate(this.alpha);
      this.font.drawString(this.text[this.pos], this.x, this.y);
    } else {
      of.Ellipse(this.x, this.y, 10, 10);
    }
  }


}

//--------------------------------------------------------------

function setup() {
  this.font = new ofxJSTrueTypeFont();
  this.font.loadFont("Scripts/workshops/esad_amiens_codesign/data/futura.ttf", 18);
}



function update(dt) {

  this.angle = dt * 30;
  println(this.particules.length);
  for (i = this.particules.length - 1; i >= 0; i--) {
    this.particules[i].update(dt);
    if (this.particules[i].life < 0) {
      this.particules.splice(i, 1);
    };
  }

}
//--------------------------------------------------------------

function draw(w, h) {
  //  println("DDD");


  of.SetColor(0, 0, 0, 30);
  of.Rect(0, 0, w, h);
  of.Translate(this.xAccroche, this.yAccroche, 0);
  of.Rotate(this.angle);
  for (i = 0; i < this.particules.length; i++) {
    //    of.PushMatrix();
    //    of.Translate(c);
    //    of.Rotate(this.angle);

    this.particules[i].draw();
    //    of.PopMatrix();

  }
}
//--------------------------------------------------------------

function onNewPacket(deviceId, volume, x, y) {
  this.volume = volume;
  this.xAccroche = x;
  this.yAccroche = y;
  this.pos++;
  if (this.pos > this.text.length-1) this.pos = 0;

  if (this.volume >= 0.5) {
    if (this.particules.length > 100) {
      //this.particules.shift();
    }
    this.particules.push(new Particule(of.Random(1000, 5000), this.font, this.pos, this.text));
  }
}