//--------------------------------------------------------------
function ParticleSquare(x,y)
{

	this.angleStart = of.Random(0,2*Math.PI);
	this.rStart = of.Random(10,15);
	this.xStart = this.rStart*Math.cos(this.angleStart);
	this.yStart = this.rStart*Math.sin(this.angleStart);

	this.x = this.xStart;
	this.y = this.yStart;


	this.vx = of.Random(-5,5);
	this.vy = of.Random(-5,5);

	this.R = 255;
	this.G = this.R;
	this.B = this.G;

	this.age = 0; // en secondes
	this.ageMax = of.Random(6,8);

	this.taille = of.Random(5,30);
	this.res = of.Random(3,10);


	this.draw = function()
	{
		of.NoFill();
		of.SetColor(this.R,this.G,this.B, 255*(1-this.age/this.ageMax));
		//of.Rect(this.x-this.taille/2, this.y-this.taille/2, this.taille,this.taille);
		of.SetCircleResolution(this.res);
		of.Ellipse(this.x, this.y, this.taille,this.taille);
		//of.Line(this.xStart,this.yStart,this.x,this.y);
	}

	this.update = function(dt)
	{
		var noise = ofNoise(this.x*0.01, this.y*0.01,dt);

		this.x += this.vx*Math.cos(2.0*noise*Math.PI); 		
		this.y += this.vy*Math.sin(2.0*noise*Math.PI); 		

		this.age += dt;
	}
}



//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup()
{
	this.particles = new Array();
	this.angleRotation = 0;
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
	this.angleRotation += 10.0*dt;

	for (var i=0 ; i < this.particles.length ; i++){
		this.particles[i].update(dt);
	}

	for (var i=this.particles.length-1; i>=0 ; i--)
	{
		if (this.particles[i].age>=this.particles[i].ageMax)
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
	//of.SetColor(0,0,0,20);
	//f.Rect(0,0,w,h);
	of.Translate(this.xSaved,this.ySaved,0);
	of.RotateZ(this.angleRotation);
	for (var i=0 ; i < this.particles.length ; i++){
		this.particles[i].draw();
	}
}


//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
//
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
	if (volume > 0.5)
	{
		for (var i=0;i<20;i++)
			this.particles.push( new ParticleSquare(x,y) );
	}

	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
