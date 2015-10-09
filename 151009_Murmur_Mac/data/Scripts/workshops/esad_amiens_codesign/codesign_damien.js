this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
this.rot = 0;
this.roz = 0;
this.start = true;

//--------------------------------------------------------------

function setup()
{
	of.Background(0,0,0);
}

//--------------------------------------------------------------

function update(dt)
{
}

//--------------------------------------------------------------

function draw(w,h)
{	
	
	if (this.start == true) { //seulement au début
		this.start = false;
		of.Background(0,0,0);
	}

	of.SetColor(255,255,255,80);
	of.NoFill();
	of.SetLineWidth(3);
	of.SetCircleResolution(this.volume*20);
	
	
	this.rot+= this.volume*20;
	of.Translate(this.xAccroche,this.yAccroche,0);
	
	of.PushMatrix();
	of.Rotate(this.rot);
	of.Ellipse(70+(this.volume*100),70+(this.volume*100), 50, 50);
	of.PopMatrix();
	of.PushMatrix();
	of.Rotate( - this.rot);
	of.Ellipse(150+(this.volume*100),150+(this.volume*150), 70, 70);
	of.PopMatrix();
	of.PushMatrix();
	of.Rotate(this.rot/2);
	of.Ellipse(200+(this.volume*100),200+(this.volume*200), 90, 90);
	of.PopMatrix();
	
}

//--------------------------------------------------------------

function onNewPacket(deviceId,volume,x,y)
{
	this.volume = volume;
	this.xAccroche = x;
	this.yAccroche = y;
}
