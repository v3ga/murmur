//--------------------------------------------------------------
// Variables à utiliser tout au cours du sketch
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
this.particules = new Array();

//--------------------------------------------------------------
function Particule(x,y,volix)
{
 	this.x = x;
	this.y = y;
	this.vx = of.Random(-10,10);
	this.vy = of.Random(-10,10);
	this.life =0;
	this.lifeMax = of.Random(3,5);
	this.vol=of.Map(volix,0,1,10,50);


	this.update = function(dt)
	{

		this.x += this.vx*15*dt; // vitesse x
		this.y += this.vy*15*dt; // vitesse y
		this.life+= dt;

		//this.y += this.vy*dt;
	}

	this.draw = function()
	{
		of.SetColor(250,250,250,of.Random(0,150));
		//of.Translate(this.xAccroche,this.yAccroche,0);
		
	of.Ellipse(this.x,this.y,this.vol,this.vol);
	//of.NoFill();
	//of.Rect(this.x-50,this.y-50,this.vol+10,this.vol+10);
		
	
		//of.Line(0,0,this.vol,this.vol);
		
		of.Ellipse(this.x+of.Random(10,60),this.y+of.Random(10,60),5,5);
	    // of.Rect( 0,0,this.vol,this.vol);
		
	}
    this.isDead = function()
    {
	if (this.life>= this.lifeMax)
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
	for(i=this.particules.length-1; i>=0 ; i--)
	{
		this.particules[i].update(dt);
		if(this.particules[i].isDead() ) 
		{
	       this.particules.splice(i,10);
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
	of.SetColor(250,250,250,of.Random(10,20));
	for(i=0;i<this.particules.length;i++)
	{
		this.particules[i].draw();
	}

	var pi,pj;
    for(i=0; i < this.particules.length; i++)
    {
    	pi =this.particules[i];
    	 for(j=i+1 ; j<this.particules.length; j++)
    	{
    	 	pj = this.particules[j];
    	 	var d = of.Dist(pi.x,pi.y,pj.x,pj.y);
    	 	if(d<=120)
    	 	{
    	 		of.Line(pi.x,pi.y,pj.x,pj.y);
    	 		
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

	if (this.volume>=0.2)
	{
		this.particules.push( new Particule(this.xAccroche,this.yAccroche,this.volume ));
	}
}