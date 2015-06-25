
this.deviceVolume = [];

//--------------------------------------------------------------
function ParticleSquare(deviceId,x,y,vol,a)
{
	this.deviceId = deviceId;
	this.x = x;
	this.y = y;
	this.vol = vol;
	this.angle=a;

	this.vx = Math.cos(a);
	this.vy = Math.sin(a);

	//this.R = of.Random(0,255);
	this.R = 255;
	this.G = 255;
	this.B = 255;

	this.taille=of.Random(2,10);
	
	this.age = 0; //en secondes
	
	this.alpha = 10;
	
	this.volModif = this.vol*of.Random(2,6);

	
	
	
	this.draw = function()
	{
//		of.Rotate(40, 1, 0, 0);
		of.SetColor(this.R,this.G,this.B,this.alpha);
		of.Ellipse(this.x,this.y,this.volModif,this.volModif);
		
	//	of.SetColor(this.R,this.G,this.B,this.alpha);
	//	of.Rect(this.x,this.y,this.taille,this.taille);
		
		
	//	of.SetColor(100,100,100,this.alpha);
	//	of.Rect(this.x,this.y,this.vol,this.vol);
	}

	this.update = function(dt)
	{
		this.x += Math.cos(this.angle)*dt*500*deviceVolume[this.deviceId]; 		
		this.y += Math.sin(this.angle)*dt*500*deviceVolume[this.deviceId];
		this.angle += 0.023;	
		this.age += dt;
		this.alpha = 255*(1-this.age/40.0);
	
	
	/*
		this.vectX=this.x;
		this.vectY=this.y;
	    this.vectX += 1;
	    this.vectY += 1;
	*/
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
	for (var i=0 ; i < this.deviceVolume.length ; i++){
		this.deviceVolume[i] += (0-this.deviceVolume[i])*0.7;
	}

	for (var i=0 ; i < this.particles.length ; i++){
		this.particles[i].update(dt);
	}
	for(var i=this.particles.length-1; i>=0 ; i--)
	{
		if(this.particles[i].age>=40)
		{
			this.particles[i].age=40;
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
	of.Background(0,0,0);
	for (var i=0 ; i < this.particles.length ; i++){
	of.SetColor(this.particles[i].R,this.particles[i].G,this.particles[i].B, this.particles[i].alpha);
	//of.Line(this.xSaved,this.ySaved,this.particles[i].x,this.particles[i].y);	
//of.Bezier(this.xSaved,this.ySaved,this.xSaved*1.2,this.ySaved*1.2,this.xSaved*1.4,this.ySaved*1.4,this.particles[i].x,this.particles[i].y);
		this.particles[i].draw();
	}
	//println("here");
	//println(this.particles.length);
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
	this.deviceVolume[deviceId] = volume;
	if (volume > 0.05)
	{
		this.particles.push( new ParticleSquare(deviceId, x,y,volume*5,of.Random(1,50) ));
	}

	this.volumeSaved = volume*200;
	this.xSaved = x;
	this.ySaved = y;
	//a=(a+1)%360
}