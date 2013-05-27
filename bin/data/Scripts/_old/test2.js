this.angle = 0;
this.volume = 0;
this.x = 0;
this.y = 0;
this.d = 0;

//--------------------------------------------------------------
function getTheme(){ return THEME_CREATURE; }

//--------------------------------------------------------------
function setup()
{
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetColor(255,255,0,255);

	this.d = this.volume*h;
	of.Ellipse(this.x,this.y,d,d);
}

//--------------------------------------------------------------
function update(dt)
{
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
    this.x = x;
    this.y = y;
	this.volume = volume;
}
