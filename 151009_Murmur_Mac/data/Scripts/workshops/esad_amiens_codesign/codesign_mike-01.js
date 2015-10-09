this.xAccroche = 0;
this.yAccroche = 0;
this.volume = 0;

this.particules = new Array(); // = {};

// -----------------------------------------------------
function Particule(x,y)
{
	this.x = x;
	this.y = y;
	this.vx = of.Random(-20,20); // Vitesse en pixels par secondes
	this.vy = of.Random(-30,30);
	this.life = 0;
	this.lifeMax = of.Random(3,6);
	
	this.xx = new Array();
	this.yy = new Array();
	this.nbmax = 100; 
	this.a = 0;
	
	this.update = function(dt)
	{
		this.a = 1-this.life/this.lifeMax;


		this.vx += of.Random(-10,10);
		this.x += this.vx*dt;
		this.vy += 40*dt; // mik---Pour simuler la gravité
		this.y += this.vy*dt;
		this.life += dt;
	
		this.xx.push( this.x );
		this.yy.push( this.y );
	
		if (this.xx.length>=this.nbmax)
		{
			this.xx.splice(0,1);
			this.yy.splice(0,1);
		}
		
	}
	
	this.draw = function()
	{
		for (var j=0;j<this.xx.length;j++)
		{
			of.SetColor(120,220,255, j/(this.xx.length-1)*255*this.a );
			of.Ellipse(this.xx[j], this.yy[j], 10, 10);
		}
//			of.Ellipse(this.x, this.y, 10, 10);
	}
	
	this.isDead = function()
	{
		if (this.life >= this.lifeMax)
		{
			return true;
		}
		return false;
	}

}


// -----------------------------------------------------
function setup()
{

}

// -----------------------------------------------------

function update(dt)
{
//	for (i=0;i<this.particules.length;i++)
	for (i= this.particules.length-1; i>=0 ; i--)

	{
		this.particules[i].update(dt);
		if ( this.particules[i].isDead() ){
			this.particules.splice(i,1);
		}
	}
}

// -----------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	for (i=0;i<this.particules.length;i++)
	{
		this.particules[i].draw();
	}
}

// -----------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	this.xAccroche = x;
	this.yAccroche = y;
	this.volume = volume;
	
	if (this.volume>=0.5)
	{
		this.particules.push( new Particule(this.xAccroche, this.yAccroche) );
	}

}




