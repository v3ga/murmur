//--------------------------------------------------------------
// Variables à utiliser tout au cours du sketch
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
this.rot = 0;
this.particules = new Array(); // nouveau tableau 

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function Particule(x,y)
{
	this.x = x;
	this.y = y;
	this.vx = 5*of.Random(-10,10); // vitesse en pixels par seconde
	this.vy = 5*of.Random(-10,10);
	this.life = 0;
	this.lifeMax = of.Random(5,10);
	this.angle = 360;
	this.d = of.Random(10,60);

	this.update = function(dt)
	{
		this.x += this.vx*dt;
		this.y += this.vy*dt;
		this.life += dt;
		this.angle += 1000*dt;
	}

	this.draw = function(dia)
	{
		//of.NoFill();
		of.SetColor(of.Random(100,255),of.Random(100,255),of.Random(100,255), 255);
		of.PushMatrix();
			of.Translate(this.x, this.y,0);
			of.Rotate(this.angle);
			of.Ellipse(this.d, 0, dia*2, dia*2);
			of.Line(0,0,this.d,0);
		of.PopMatrix();

	}

	this.isDead = function()
	{
	if(this.life >= this.lifeMax)
		return true;
	return false;

	}

}

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup()
{



}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
	for(i=this.particules.length-1;i>=0 ; i--)
	{
		this.particules[i].update(dt);
		if(this.particules[i].isDead())
		{
			this.particules.splice(i,1);

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
	of.SetColor(255,255,255);
	for(i=0;i<this.particules.length;i++)
	{
		this.particules[i].draw(this.volume*50);
	}

	var pi,pj;
	for(i=0;i<this.particules.length;i++)
	{
		pi = this.particules[i];
		for(j=i+1;j<this.particules.length;j++)
		{
			pj= this.particules[j];
			var d= of.Dist(pi.x,pi.y, pj.x, pj.y);
			if(d<=300)
			{
				of.Bezier(pi.x, pi.y, pj.x, pj.y, pi.x, pi.y, pj.x, pj.y);



			}

		}

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
	
	this.xAccroche = x;
	this.yAccroche = y;
	this.volume = volume;


	if (this.volume>=0.5)
	{
		this.particules.push(new Particule(this.xAccroche,this.yAccroche));
	


	}



}
