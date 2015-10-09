//--------------------------------------------------------------
// Declaration des variables d'instance
//
//var font = new ofxJSTrueTypeFont();
//font.loadFont("Scripts/symbol.ttf",100);  // 
//font.loadFont("Scripts/data/futura.ttf",100);
MAX_AGE=500; // const MAX_AGE=500;

this.x=0;
this.y=0;

function ParticleSquare(x,y) {
	this.x = x;
	this.y = y;
	this.vx = of.Random(-3,3);
	this.vy = of.Random(-3,3);
	this.r = of.Random(0,255);
	this.g = of.Random(100,255);
	this.b = of.Random(200,255);
	this.age=0;
	this.typeOfParticle = of.Random(0,100);
	this.size = 50;
	this.dying = false;
	this.tracingdie = false;

	this.draw = function(w,h)
	{
			of.SetColor(this.r, this.g, this.b, this.transp);
			if (this.typeOfParticle <= 45)
				of.Rect(this.x, this.y, this.size, this.size);
			else if (this.typeOfParticle <= 78)
				of.Circle(this.x, this.y, this.size/2);
			else // if (this.typeOfParticle == 5)
//				if (font)
//					font.drawString("*",this.x,this.y)
	}
	
	this.update = function(dt)
	{
		if (this.age < MAX_AGE/2) {
			this.x += this.vx;
			this.y += this.vy;
		} else {
			if (this.age > MAX_AGE/4) {
				if (this.dying == false) {
					this.dying = true;
					this.tracingdie = (of.Random(1,100)>90);
				}
			}
			this.r = 255; this.g = 50; this.b = 50;
		}
		this.age += of.Random(1,3);
		if (this.tracingdie) {
			// this.transp = (100*(MAX_AGE-this.age)/255)-50;
			// this.x += this.x;
			this.y += +5;
		} else {
			this.transp = (100*(MAX_AGE-this.age)/255)-50;
			this.x += of.Random(-10,10);
			this.y += of.Random(-10,10);
		}
	}

}

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup()
{
	// on crée une liste de particules
	this.particles = new Array();
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
	for (var i = 0 ; i < this.particles.length ; i++) {
		this.particles[i].update(dt);
	}
	for (var i=this.particles.length-1 ; i>=0; i--) { // i-- parceque c plus performant pour le resize de l'array pâr splice()
		if (    ((this.particles[i].age>=MAX_AGE) && (! this.particles[i].tracingdie))
			 || (this.particles[i].age>=MAX_AGE*3)) {
				this.particles.splice(i,1);  // enlève 1 élément à partir de 1 (et resize de l'array)
		}
	}
}

//--------------------------------------------------------------
// Appel pour le dessin sur la surface
//
// (w,h) sont les dimensions de la surface en pixels
//
// fonction de dessin (et autres)
// https://code.google.com/p/ofxjavascript/wiki/ofxJSGlobalFunctions
//
function draw(w,h)
{
	of.Background(0,0,0);
	for (var i = 0 ; i < this.particles.length ; i++) {
		//of.SetColor(255,255,255,200);
		//of.Line(this.xSaved, this.ySaved, this.particles[i].x, this.particles[i].y);
		this.particles[i].draw(w,h);
		if (this.particles[i].tracingdie && this.particles[i].age <= MAX_AGE/10) {
			of.SetColor(255,50,50,150);
			if (i-1>=0) {
				of.Line(this.particles[i-1].x, this.particles[i-1].y,this.particles[i].x, this.particles[i].y);
			}
		}
	}
//	println(this.particles.length);
//	print(this.volumeSaved+"\n");
}


//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
	// println("event onNewPacket : "+volume);
	if (volume>0.4) { 
		this.particles.push(new ParticleSquare(x,y));
		this.particles.push(new ParticleSquare(x,y));
	}
	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
