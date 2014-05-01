this.xAccroche = 0;
this.yAccroche = 0;
this.volume = 0;
this.particules = new Array(); // =[]
	 


function Particule(x,y)
{
	this.x = x;
	this.y =y;
	this.vx = of.Random(-10,10)*volume*10; // vitesse en pixels par sec
	this.vy = of.Random(-10,10)*volume*10;
	this.r = of.Random(100,255);
	this.v = of.Random(100,255);
	this.b = of.Random(100,255);
 	this.alpha = of.Random(80,100);
	this.life = 0;
	this.lifeMax = of.Random(6,10);
	this.isComeBack = false;
	var dia = 20;

	this.update = function(dt)
	{
		this.x += this.vx*dt;
		this.y += this.vy*dt;
		this.life += dt;	
	
	
	
	if(this.life>= 0.7*this.lifeMax && !this.isComeBack){
		this.vx= -this.vx;this.vy= -this.vy;
		this.isComeBack = true;
			this.dia -= 10;
		};
		
	}
	
	this.draw = function()
	{
 		of.SetColor(this.r,this.v,this.b,this.alpha);
		of.Ellipse(this.x, this.y,-dia,dia);
	}
	
	this.isDead = function()
	{
	if (this.life >= this.lifeMax){
		return true;
		}
		return false;	
	}
	
}

// -------------------------------------------------
function setup()
	{
		
	}
// -------------------------------------------------

function update(dt)
	{
	//for(i=0;i<this.particules.length;i++)
	for (i=this.particules.length-1; i>=0; i--)
		{
			this.particules[i].update(dt);
			if (this.particules[i].isDead()){
				this.particules.splice(i,1);
			}
		}	
	}	
// -------------------------------------------------
	
function draw(w,h)
	{
 	of.Background(0,0,0);
	of.SetColor(255,255,255,40);
	var pi,pj;
		
	for(i=0;i<this.particules.length;i++)
	{
		pi=this.particules[i];
		for(j=i+1; j < this.particules.length;j++){
			pj=this.particules[j];
			var d = of.Dist(pi.x,pi.y,pj.x,pj.y);
			if (d<=100){
				of.Line(pi.x,pi.y,pj.x,pj.y);
				}
			}
		}	

	
	for(i=0;i<this.particules.length;i++)
		{
			this.particules[i].draw();
		}

		
		

	}	

// -------------------------------------------------
	
function onNewPacket(deviceId,volume,x,y)
	{
		this.xAccroche =x;
		this.yAccroche =y;
		this.volume = volume;
		
		if(volume >= 0.2)
		{
			this.particules.push(new Particule(this.xAccroche,this.yAccroche));
		}
		
	}	
	
	
	
	
	