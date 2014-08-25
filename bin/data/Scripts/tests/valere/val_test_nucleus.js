//--------------------------------------------------------------
this.rot = 0;
this.light = null;


function ParticleSquare(x,y)
{
	this.x = x;
	this.y = y;
	
	this.vx = of.Random(-7,7);
	this.vy = of.Random(-7,7);

	

	this.R = of.Random(140,150);
	this.G = of.Random(150,255);
	this.B = of.Random(0,100);

	this.mouv = 50;

	this.age = 0;
	

	this.taille = (10);
	

	this.draw = function()
	{
		of.SetColor(this.R,this.G,this.B, 255*(4-this.age/4.0));
		of.Rect(this.x-this.taille/2, this.y-this.taille/2, 1,1);
		//of.Ellipse(this.x, this.y, this.taille,this.taille);

	}

	this.update = function(dt)
	{
		var noise = ofNoise(this.x*0.01, this.y*0.01,dt*250);
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
	this.light = new ofxJSLight;
	
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps Ã©coulÃ© depuis le dernier dessin (en seconde)
//
function update(dt)


{	this.rot += 45.0*dt;
	
	for (var i=0 ; i < this.particles.length ; i++){
		this.particles[i].update(dt);
	}

	for (var i=this.particles.length-1; i>=0 ; i--)
	{
		if (this.particles[i].age>=3)
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
	
	//of.SetColor(0,0,0);
	//of.Rect(0,0,w,h);
	
	of.EnableLighting();
	
	
	
	
	this.light.setPointLight ();
	this.light.enable(); 
	
	// frame blending
of.EnableSmoothing();
	
	of.Rect(0,0,w,h);
of.Background(0,0,0);

	
	for (var i=0 ; i < this.particles.length ; i++){
		//of.Line(this.xSaved,this.ySaved,this.particles[i].x,this.particles[i].y);

		//this.particles[i].draw();
	//of.DrawBox (this.xSaved,this.ySaved,200,100,this.volumeSaved*100,50);
	//of.DrawCylinder (this.xSaved-200,this.ySaved+200,this.volumeSaved*150,100);
	this.particles[i].draw();
	
	//of.DrawPlane  (this.xSaved-50,this.ySaved+100,this.volumeSaved*250,200);
	
	//for (var i=0 ; i < this.particles.length ; i++){
	of.Line(this.xSaved,this.ySaved,this.particles[i].x,this.particles[i].y);
		
of.DrawSphere   (this.particles[i].x,this.particles[i].y,6);
		
		//of.DrawCone (this.particles[i].x,this.particles[i].y,10,10,10);
	
	
	
		




	//}
		
	//println(this.iii);
}
	
		of.NoFill();
	of.SetColor (106,87,230);
	of.PushMatrix();
		of.Translate(this.xSaved,this.ySaved,0);
	of.RotateX(this.rot);
	of.RotateY(this.rot);
	

	of.DrawIcoSphere (0,0,0,this.volumeSaved*100);
	this.light.disable();
	of.DisableLighting();
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
