
//--------------------------------------------------------------
function setup()
{
	this.x = new Array();
	this.y = new Array();
	this.angle = 0;
	this.R = 200.0;
	this.sizeMax = 500;
	this.Rmodulate = 0;
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetColor(255);

	of.PushMatrix();
	of.Translate(w/2,h/2,0);
	if (this.x.length>=2)
	{
		for (var i=0;i<this.x.length-1;i++)
		{
			of.Line(x[i],y[i],x[i+1],y[i+1]);
		}
	}
	of.PopMatrix();
}

//--------------------------------------------------------------
function update(dt)
{
	this.angle += 40*dt;
	this.x.push( (this.R + this.Rmodulate) * Math.cos(Math.PI*this.angle/180.0) );
	this.y.push( (this.R + this.Rmodulate) * Math.sin(Math.PI*this.angle/180.0) );

	if (this.x.length>this.sizeMax)
	{
		this.x.splice(0,1);
		this.y.splice(0,1);
	}
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	this.Rmodulate = volume*100;
}
