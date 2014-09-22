//--------------------------------------------------------------
function ParticleSquare(x,y)
{

	this.angleStart = of.Random(0,2*Math.PI);
	this.rStart = of.Random(10,15);
	this.xStart = this.rStart*Math.cos(this.angleStart);
	this.yStart = this.rStart*Math.sin(this.angleStart);

	this.x = this.xStart;
	this.y = this.yStart;
	
	this.sizeRandomMin = 5;
	this.sizeRandomMax = 30;

	this.speedRandomMin = -2;
	this.speedRandomMax = 2;

	this.vx = of.Random(this.speedRandomMin,this.speedRandomMax);
	this.vy = of.Random(this.speedRandomMin,this.speedRandomMax);

	this.R = 255;
	this.G = this.R;
	this.B = this.G;

	this.age = 0; // en secondes
	this.ageMax = of.Random(6,12);

	this.taille = of.Random(this.sizeRandomMin,this.sizeRandomMax);
	this.res = of.Random(3,10);
	
	this.noiseFactor = 0.01;
	this.noiseFactorTime = 0.01;
	this.noiseOffx = of.Random(-1,1);
	this.noiseOffy = of.Random(-1,1);


	this.setNoiseFactor = function(f_,t_)
	{
		this.noiseFactor = f_;
		this.noiseFactorTime = t_;
	}

	this.setSizeRandomMinMax = function(min_, max_)
	{
		this.sizeRandomMin = min_;
		this.sizeRandomMax = max_;

		this.taille = of.Random(this.sizeRandomMin,this.sizeRandomMax);
	}

	this.setSpeedRandomMinMax = function(min_, max_)
	{
		this.speedRandomMin = min_;
		this.speedRandomMax = max_;

		this.vx = of.Random(this.speedRandomMin, this.speedRandomMax);
		this.vy = of.Random(this.speedRandomMin,this.speedRandomMax);
	}

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
		var noise = ofNoise(this.noiseOffx+this.x*this.noiseFactor, this.noiseOffy+this.y*this.noiseFactor,dt*this.noiseFactorTime);

		this.x += this.vx*Math.cos(2.0*noise*Math.PI);
		this.y += this.vy*Math.sin(2.0*noise*Math.PI);

		this.age += dt;
	}
}

//--------------------------------------------------------------
function createUI()
{
    this.newSlider("noiseFactor",0.01,0.1, 0.01);
    this.newSlider("noiseFactorTime",0.01,0.1, 0.01);
    this.newSlider("sizeRandomMin",3,10, 5);
    this.newSlider("sizeRandomMax",10,40, 20);
    this.newSlider("speedRandomMin",-5,0, -1);
    this.newSlider("speedRandomMax",0,5, 1);
    this.newSlider("angleRotation",0,20, 10);
}

//--------------------------------------------------------------
function eventUI(name, value)
{
	if (name == "noiseFactor")
	{
		this.noiseFactor = value;
	}
	else if (name == "noiseFactorTime")
	{
		this.noiseFactorTime = value;
	}

	else if (name == "sizeRandomMin")
 	{
		this.sizeRandomMin = value;
	}
	else if (name == "sizeRandomMax")
	{
		this.sizeRandomMax = value;
	}
	else if (name == "speedRandomMin")
	{
		this.speedRandomMin = value;
	}
	else if (name == "speedRandomMax")
	{
		this.speedRandomMax = value;
	}
	else if (name == "angleRotation")
	{
		this.angleRotationSpeed = value;
	}
}

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
this.xSaved=0;
this.ySaved=0;
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
	if (this.angleRotationSpeed == null) return;
	
	this.angleRotation += this.angleRotationSpeed*dt;

	for (var i=0 ; i < this.particles.length ; i++){
		this.particles[i].update(dt);
	}

	for (var i=this.particles.length-1; i>=0 ; i--)
	{
		if (this.particles[i].age>=this.particles[i].ageMax)
		{
			this.particles[i].age = this.particles[i].ageMax;
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
	if (this.xSaved>0 && this.ySaved>0)
	{
		of.Translate(this.xSaved,this.ySaved,0);
		if (this.angleRotation != null)
			of.RotateZ(this.angleRotation);
		for (var i=0 ; i < this.particles.length ; i++){
			this.particles[i].draw();
		}
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
		for (var i=0;i<5;i++)
		{
			var p = new ParticleSquare(x,y);
			
			p.setNoiseFactor(this.noiseFactor, this.noiseFactorTime);
			p.setSizeRandomMinMax(this.sizeRandomMin, this.sizeRandomMax);
			p.setSpeedRandomMinMax(this.speedRandomMin, this.speedRandomMax);
			this.particles.push( p );
		}
	}

	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
