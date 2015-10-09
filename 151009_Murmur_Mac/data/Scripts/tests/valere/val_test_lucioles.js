//--------------------------------------------------------------
function ParticleSquare(x,y)
{
	this.x = x;
	this.y = y;
	
	this.vx = of.Random(-7,7);
	this.vy = of.Random(-7,7);

	

	this.R = of.Random(200,255);
	this.G = of.Random(200,255);
	this.B = of.Random(0,20);

	this.mouv = of.Random(0,500);

	this.age = 0;
	

	this.taille = of.Random(0,8);
	

	this.draw = function()
	{
		of.SetColor(this.R,this.G,this.B, 255*(4-this.age/2.0));
		of.Rect(this.x-this.taille/2, this.y-this.taille/2, 1,1);
		of.Ellipse(this.x, this.y, this.taille,this.taille);


	}

	this.update = function(dt)
	{
		var noise = ofNoise(this.x*0.001, this.y*0.001,dt*25);
		this.x += this.vx*Math.cos(3.5*noise*noise*Math.PI); 		
		this.y += this.vy*Math.sin(3.5*noise*noise*Math.PI); 		
		this.age += dt;
	}
}



//--------------------------------------------------------------
// Appel au dÃ©marrage de l'animation
//
this.iii=0;
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
		if (this.particles[i].age>=100)
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

	of.SetColor(0,0,0);
	of.Rect(0,0,w,h);


	for (var i=0 ; i < this.particles.length ; i++){
		//of.Line(this.xSaved,this.ySaved,this.particles[i].x,this.particles[i].y);

		this.particles[i].draw();




	}
	//println(this.iii);
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
