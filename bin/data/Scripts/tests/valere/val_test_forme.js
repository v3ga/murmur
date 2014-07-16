
	
	function ParticleSquare(x,y)
	{ 
		this.x = x;
		this.y = y;

		this.vx = of.Random(-2,2);
		this.vy = of.Random(-2,2);

		this.R = (0);
		this.G = of.Random(0,255);
		this.B = of.Random(0,255);

		this.age = 0;
		this.ageMax = of.Random(1,5);
		
		this.taille = of.Random(2,20);

		this.draw = function()
		{
			of.SetColor(this.R,this.G,this.B, 255*(1-this.age/this.ageMax));
			of.NoFill();
			//of.SetLineWidth(3);
			of.EnableSmoothing  ();
			of.SetCircleResolution (3+this.age*6);
			of.Circle(this.x,this.y,this.taille+this.age*75);

		}

		this.update = function(dt)
		{
			var noise = ofNoise(this.x*0.05, this.y*0.05,dt*100);
			
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

}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//


function update(dt)
{
	for (var i=0; i<this.particles.length; i++)
	{
		this.particles[i].update(dt);
	}

	for (var i=this.particles.length-1; i>=0; i--)
	{
		if (this.particles[i].age>=this.particles[i].ageMax)
			this.particles.splice(i,1);
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
	of.SetColor(0,0,0,2);
	of.Rect(0,0,w,h);



	for (var i=0; i<this.particles.length; i++)
{
	//of.Line(this.xSaved, this.ySaved, this.particles[i].x, this.particles[i].y);
		this.particles[i].draw();
		}




}


//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//


function onNewPacket(deviceId,volume,x,y)
{
	if (volume >0.5)
	{
		this.particles.push(new ParticleSquare(x,y) );
	}

	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved=y;
}
