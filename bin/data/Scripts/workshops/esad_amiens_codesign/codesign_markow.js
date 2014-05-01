//--------------------------------------------------------------
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
//this.soundcolor = 0;
this.particules = new Array();

//--------------------------------------------------------------
function particulo(x,y,monVol)
{
	this.x= x;
	this.y= y;
	this.vx = of.Random(-50,20);// en pixel par sec
	this.vy = of.Random(-50,20);
	this.life=0;
	this.lifeMax = of.Random(15,30);
	this.leVol= of.Map(monVol,0,1,0,200);
	this.opac= of.Map(monVol,0,1,5,40);
	
	
	
	this.update = function(dt){
		this.x += this.vx*dt;
		this.y += this.vy*dt;
		this.life+=dt;	
		
	}
	this.draw =function(){	
	    of.SetColor(255,255,255,this.opac);
		of.Ellipse(this.x,this.y,this.leVol,this.leVol);
	}
	
	this.isdead =function(){
		if(this.life>= this.lifeMax){
			return true;	
		}
				return false;
		
		
	}

}



function setup()

{

}

//--------------------------------------------------------------

function update(dt)
{
	
	
	//for(i=0;i<this.particules.length;i++)
	for(i=this.particules.length-1;i>=0;i--)
	{
		//this.soundcolor = of.Map(this.volume,0,1,0,100);
		
		this.particules[i].update(dt);
		
		
		if(this.particules[i].isdead()){
		this.particules.splice(i,1);
		}
		
	}
	
}

//--------------------------------------------------------------

function draw(w,h)
{
	
	 of.Background(0,0,0);
	 //of.SetColor(255,255,255,100);



 	for(i=0;i<this.particules.length;i++)
	{
		
		
	 //this.soundcolor = of.Map(this.volume,0,1,0,255);
	 //of.SetColor(this.soundcolor,0,255,100);
	 //of.SetLineWidth(1);
	 //of.NoFill();
		this.particules[i].draw();
		
	}
	
	/*	//-------------- Liens entre particules -----
	var pi, pj;
	for(i=0;i<this.particules.length;i++)
	{
		pi=this.particules[i];
	    for(j=i+1; j<particules.length; j++)
	    {
			pj= this.particules[j];
			var d = of.Dist(pi.x,pi.y,pj.x,pj.y);
			if(d<=10){
				of.Line(pi.x,pi.y,pj.x,pj.y);
		}
	}	
}


}

*/

}

//--------------------------------------------------------------

function onNewPacket(deviceId,volume,x,y)
{
	this.volume = volume;
	this.xAccroche = x;
	this.yAccroche = y;
	
	//this.soundcolor = of.Map(this.volume,0,1,0,200);
	if(this.volume >=0.1){
		this.particules.push(new particulo(this.xAccroche,this.yAccroche,this.volume));
	}
	

}
