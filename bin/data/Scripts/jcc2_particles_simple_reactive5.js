//--------------------------------------------------------------
function ParticleSquare(x,y,a)
{
	this.x = x;
	this.y = y;
	this.a = a;
	
	this.vx = Math.cos(a)*3;
	this.vy = Math.sin(a)*3;
	
	this.taille = 50;

	this.R = 50;
	this.G = of.Random(125,255)
	this.B = 255;
	
	this.age = 0; // en secondes
	this.ageMax = of.Random(0,10);

	this.draw = function()
	{
		of.NoFill();
		of.SetColor(this.R,this.G,this.B, 255*(1-this.age/4.0));
		of.Ellipse (this.x,this.y,this.taille*2.5,this.taille*2.5);
	
		of.SetColor(this.R,this.G,this.B, 255*(1-this.age/2.0));
		of.Ellipse (this.x+1,this.y+1,this.taille*2.5,this.taille*2.5);
		
		of.SetColor(this.R,this.G,this.B, 255*(1-this.age/2.0));
		of.Ellipse (this.x+13,this.y+13,this.taille,this.taille);
		

	}

	this.update = function(dt)
	{
		this.x += this.vx; 		
		this.y += this.vy; 	
		
		this.age += dt;
		
	}
}



//--------------------------------------------------------------
// Appel au dÃ©marrage de l'animation
//
function setup()
{
	this.particles = new Array();
	this.a = 0;
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
		if (this.particles[i].age>=this.particle[i].ageMax)
		{
			this.particles[i].age = 4.0;
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
	if (volume > 0.17)
	{
		this.particles.push( new ParticleSquare(x,y,a) );
	}

	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
	a=(a+1)%180;
}
