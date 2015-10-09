



	this.xAccroche = 0;
	this.yAccroche = 0;
	this.volume = 0;
	this.rot = 0;
	
	this.particules = new Array(); // = {};
	
	
//.……....................................................
function Particule(x,y) // Cette fonction correspond au constructeur
{
	this.x = x;
	this.y = y;
	this.vx = of.Random(-60,60); // Vitesse en pixels par seconde
	this.vy = of.Random(-60,60);
	this.life = 0;
	this.lifeMax = of.Random(40,80);
	this.angle = of.Random(10,30);
	this.d = of.Random(16,33);
	
	this.update = function(dt) // On ajoute une méthode à notre class particule
	{
		this.x += this.vx*dt;
		this.y += this.vy*dt;
		this.life += dt; // dt représente le temps écoulé entre deux dessins
		this.angle += 80*dt;
	}
	
	this.draw = function()
	{
		of.NoFill();
		of.Ellipse(this.x, this.y, of.Random(0,20), of.Random(0,20))
		of.Circle(this.x, this.y, of.Random(0,20))
		
		of.PushMatrix();
		of.Translate(this.x,this.y,0);
		of.Rotate(this.angle);
		of.Ellipse(this.d, 0, of.Random(0,20), of.Random(0,20))
		//of.Circle(this.d, 0, of.Random(0,20))
		of.PopMatrix();
		
	}
	
	this.isDead = function()
	{
		if(this.life >= this.lifeMax)
		return true;
		return false;
	}
}

//.……....................................................
function setup()
{

}


//.……....................................................
function update(dt)
{
	// for (i=0; i<this.particules.length; i++) // length = nombre de particules créées
	for (i= this.particules.length-1; i>=0; i--)
	{
		this.particules[i].update(dt);
		if (this.particules[i].isDead() )
		{
			this.particules.splice(i,3);
		}
	}
}


//.……....................................................
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetColor(of.Random(100,250));

	for (i=0; i<this.particules.length; i++) // length = nombre de particules créées
	{
		this.particules[i].draw();
	}
	
		var pi,pj;
		for (i=0; i<this.particules.length; i++)
		{
			pi = this.particules[i];
			for (j=i+1; j<this.particules.length; j++)
			{
				pj = this.particules[j];
				var d = of.Dist(pi.x,pi.y,pj.x,pj.y);
				if (d<=90){
					of.Line(pi.x,pi.y,pj.x,pj.y);
				}
			}	
		}

}


//.……....................................................
function onNewPacket(deviceId,volume,x,y)
{
	this.xAccroche = x;
	this.yAccroche = y;
	this.volume = volume;
	
	if (this.volume >= 0.3)
	{
		this.particules.push( new Particule(this.xAccroche,this.yAccroche) );
	}
}