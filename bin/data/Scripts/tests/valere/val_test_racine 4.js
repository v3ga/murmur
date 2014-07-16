this.R = 255;
this.G = 255;
this.B = 255;
this.facteurNoise = 100;
this.circleSize = 1;

function createUI()
{
    this.newSlider("color_R",0,255,this.R);
    this.newSlider("color_G",0,255,this.G);
    this.newSlider("color_B",0,255,this.B);
    this.newSlider("Noise",0,150,this.facteurNoise);
    this.newSlider("Size",1,20, this.circleSize);
   
}

function eventUI(name, value)
{
    if (name == "color_R")
    {
        this.R = value;
    }
    else if (name == "color_G")
    {
        this.G = value;
    }
    else if (name == "color_B")
    {
        this.B = value;
    }
    else if (name == "Noise")
    {
        this.facteurNoise = value;
    }
    else if (name == "Size")
	{
		this.circleSize = value;
	}
   
}

//--------------------------------------------------------------
function ParticleSquare(sketch, x,y)
{
    this.sketch = sketch;
    
	this.x = x;
	this.y = y;
	
	this.vx = of.Random(-7,7);
	this.vy = of.Random(-7,7);

    this.R = this.sketch.R;
    this.G = this.sketch.G;
    this.B = this.sketch.B;
    this.facteurNoise = this.sketch.facteurNoise;
    this.circleSize = this.sketch.circleSize;
 
    //---------------------------------------------------------
    

	this.mouv = of.Random(0,500);
    this.age = 0;
	
	
//---------------------------------------------------------------
	this.draw = function()
	{
		of.SetColor(this.R,this.G,this.B, 255*(4-this.age/4.0));
		of.Rect(this.x-this.taille/2, this.y-this.taille/2, 1,1);
		of.Ellipse(this.x, this.y, this.circleSize,this.circleSize);
		
		
	}

	this.update = function(dt)
	{
		var noise = ofNoise(this.x*0.01, this.y*0.01,dt*this.facteurNoise);
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
	of.SetColor(0,0,0,4);
	of.Rect(0,0,w,h);

	for (var i=0 ; i < this.particles.length ; i++){
		
        this.particles[i].draw();
    }

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
		this.particles.push( new ParticleSquare(this, x,y) );
	}

	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
