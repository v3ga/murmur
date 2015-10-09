this.volume = 0;
this.xAcc = 0;
this.yAcc = 0;
this.listeParticules = Array();
this.width = 1200;
this.height = 900;

function particule (x_,y_)
{
	this.x = x_;
	this.y = y_;
	this.vx = of.Random(-100,100);
	this.vy = of.Random(-100,100);
	this.age = 0;

	this.update = function(dt)
	{
		this.age += dt;
		this.x += this.vx*dt;
		this.y += this.vy*dt;

		if(this.x >= width){
			this.vx = -this.vx;
		}
		if(this.x <= 0){
			this.vx = -this.vx;
		}



		if(this.y >= height){
			this.vy = -this.vy;
		}
		if(this.y <= 0){
			this.vy = -this.vy;
		}

	}

	this.draw = function()
	{
		of.SetColor(255,255,255,100);
		of.Ellipse(this.x,this.y,20,20);

	}
}


function setup()
{

}

function update(dt)
{
	for(var i=0; i<this.listeParticules.length; i++)
	{
		this.listeParticules[i].update(dt);
	}


		for(var i=this.listeParticules.length-1 ; i>=0; i--){

			if(this.listeParticules[i].age >= 7.0){
				this.listeParticules.splice(i,1);
			}
		}


}

function draw(w,h)
{

	of.Background(0,0,0);
	of.SetColor(0,255,0,255);
//	of.Ellipse(this.xAcc, this.yAcc, 100,100);

	for(var i=0; i<this.listeParticules.length; i++){
		this.listeParticules[i].draw();
	}

		var pi, pj;
	for(var i=0; i<this.listeParticules.length; i++){
		pi = this.listeParticules[i];
		for (var j=i+1; j<this.listeParticules.length;j++ )
		{
			pj = this.listeParticules[j];
			if (of.Dist(pi.x,pi.y,pj.x,pj.y)<= 100 )
			{
				of.Line(pi.x,pi.y,pj.x,pj.y);
			}
		}
	}


}


function onNewPacket(deviceId,volume,x,y)
{
	this.volume = volume;
	this.xAcc = x;
	this.yAcc = y;
	if(volume >= 0.5)
	{
		//this.listeParticules.push(new particule(x,y) );
		var nb = 10;
		var angle = of.Random(0,Math.PI);
		for(var i=0; i<nb; i++)
		{
			var p = new particule(x,y);
			p.vx = 150*Math.cos(angle);
			p.vy = 150*Math.sin(angle);
			this.listeParticules.push(p);
			angle += 2*Math.PI/nb;

		}

	}
}