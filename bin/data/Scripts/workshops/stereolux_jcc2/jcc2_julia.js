//--------------------------------------------------------------
function ParticleSquare(x,y)
{
	this.x = x;
	this.y = y;

	this.vx = of.Random(-5,5);
	this.vy = of.Random(-5,5);

	this.R = of.Random(0,255);
	this.G = this.R; // of.Random(0,255);
	this.B = this.R; //of.Random(0,255);

	this.mouv = of.Random(0,255);

	this.age = 0;
	this.taille = of.Random(5,30);

	this.draw = function()
	{
		of.SetColor(this.R,this.G,this.B, 255*(1-this.age/4.0));
		of.Rect(this.x-this.taille/2, this.y-this.taille/2, 1,1);
	}

	this.update = function(dt)
	{
		var noise = ofNoise(this.x*0.01, this.y*0.01,dt);
		this.x += this.vx*Math.cos(2.0*noise*noise*Math.PI); 		
		this.y += this.vy*Math.sin(2.0*noise*noise*Math.PI); 		
		this.age += dt;
	}
}



//--------------------------------------------------------------
// Appel au dÃ©marrage de l'animation
//
function setup()
{
	this.particles = new Array();
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps Ã©coulÃ© depuis le dernier dessin (en seconde)
//
function update(dt)
{
	for (var i=0 ; i < this.particles.length ; i++){
		this.particles[i].update(dt);
	}

	for (var i=this.particles.length-1; i>=0 ; i--)
	{
		if (this.particles[i].age>=4)
		{
			this.particles.splice(i,1);
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
	//of.Background(0,0,0);
	// frame blending
	of.SetColor(0,0,0,8);
	of.Rect(0,0,w,h);
	for (var i=0 ; i < this.particles.length ; i++){
		of.Line(this.xSaved,this.ySaved,this.particles[i].x,this.particles[i].y);
		this.particles[i].draw();
	}
	//println("here");
	println(this.particles.length);
}


//--------------------------------------------------------------
// Appel lorsque la surface reÃ§oit un nouveau Â«packetÂ» lumineux
//
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transportÃ©, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
	if (volume > 0.5)
	{
		this.particles.push( new ParticleSquare(x,y) );
	}

	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
