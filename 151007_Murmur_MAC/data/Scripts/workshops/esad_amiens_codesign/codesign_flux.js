this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;

this.particules = new Array(); // création du tableau

//---------------------------------------------
//creation de la "class" particule
function Particules(x,y) 
{
		this.xDepart = x;
		this.yDepart = y;
		this.x = x;
		this.y = y;
		this.vx = of.Random(-1000,1000); //vitesse de la particule en px/s
		this.vy = of.Random(-1000,1000); //vitesse de la particule en px/s
		

		
		this.update = function(dt) // méthode de la classe
		{
			this.x += this.vx*dt;	
			this.y += this.vy*dt;
			this.life += dt;
			
		}
		
		
		this.draw = function()
		{
			of.SetLineWidth(5);
			of.Line(this.xDepart,this.yDepart,this.x,this.y);	
			
							
		}

}

//---------------------------------------------

function setup()
{
	
}


//---------------------------------------------

function update(dt)
{
	
		println("hello world")

	
	for(i=0; i<this.particules.length; i++)
	{
		this.particules[i].update(dt);
				
		
	}
	
}


//---------------------------------------------

function draw(w,h)
{
	
	of.Background(0,0,0);
	
	
	//blanc
	//of.SetColor(255,255,255,100);
	
	
	of.SetColor(of.Random(0,150),of.Random(0,150),of.Random(0,150),80);
	
	for(i=0; i<this.particules.length; i++)
	{
		this.particules[i].draw();
	}
	
	
	var pi,pj;
	for(i=0; i<this.particules.length; i++)
{
	pi=this.particules[i];
	for(j=i+1; j<this.particules.length; j++){
		pj=this.particules[j];
		var d =of.Dist(pi.x,pi.y,pj.x,pj.y);
		if(d<=1000){
			of.Line(pi.x,pi.y,pj.x,pj.y)
			}
		
		}
	}
	
	}



//---------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	
	this.xAccroche = x;
	this.yAccroche = y;
	this.volume = volume;
	
	if (this.volume>=0.5)
	{
		this.particules.push( new Particules(this.xAccroche,this.yAccroche));	
		
	}
	
	
}