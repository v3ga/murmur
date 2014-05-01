//--------------------------------------------------------------

this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;

this.colori=0;



this.particules= new Array();



//---------------------------------------------------------------

function particule(x,y){
this.x=x;
this.y=y;
//this.vx= of.Random(-10,10);
this.vx= 2;
//this.vy= of.Random(-10,10);
this.vy= 2;
this.p= of.Random(0,4);
this.t = of.Random(0,5);
this.f = of.Random(0.005,0.02);

this.life = 0;
this.lifeMax = of.Random(1,8);


//var col= of.Map(this.volume,0,1,0,255);
//this.colori= col;
  //  this.dif=-1;
  //   this.colori+=this.dif;

this.colori=this.volume*100;


//----------------------------
this.update = function(dt)
{
var n = ofNoise(this.x*this.f, this.y*this.f, this.t);
this.t += dt;
this.vx = 100*Math.cos(n*2*Math.PI*this.p);

this.vy = 100*Math.sin(n*2*Math.PI*this.p);

this.x += this.vx*dt;
this.y += this.vy*dt;

this.life+=dt;

}

//-----------------------------
this.isDead = function()
{
	if (this.life >= this.lifeMax)
		return true;
	return false;
}
//-----------------------------
this.draw = function(ta){

of.Ellipse(this.x,this.y,10+ta,10+ta);

}


}



//--------------------------------------------------------------

function setup()
{

}

//--------------------------------------------------------------

function update(dt)
{
for(var i=this.particules.length-1;i>=0;i--)
{
	this.particules[i].update(dt);
	if (this.particules[i].isDead())
		this.particules.splice(i,1);
}



}

//--------------------------------------------------------------

function draw(w,h)
{
    of.Background(0,0,0);
    this.colori=this.volume*100;
    this.rand= of.Random(100,150);
    this.rand2= of.Random(100,150);
    this.rand3= of.Random(100,150);
    of.SetColor(this.colori,this.colori,this.rand3,100);
   
    //of.SetColor(255);
     
    

    // color();
       
       
    for(i=0;i<this.particules.length;i++){
this.particules[i].draw(this.volume*190);
}


var pi,pj;
 for(i=0;i<this.particules.length;i++){
 pi=this.particules[i];
 
 
  for(j=i+1;j<this.particules.length;j++){
  
	pj= this.particules[j];
	var d = of.Dist(pi.x,pi.y,pj.x,pj.y);
	if(d<=100){
	of.Line(pi.x,pi.y,pj.x,pj.y);
	
	}
	}
}
}

//--------------------------------------------------------------

function onNewPacket(deviceId,volume,x,y)
{
	this.volume = volume;
	this.xAccroche = x;
	this.yAccroche = y;
	
	if(this.volume>=0.5){
	this.particules.push(new particule(this.xAccroche,this.yAccroche));
	}
}
