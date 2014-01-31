
//--------------------------------------------------------------
function setup()
{
	this.font = new ofxJSTrueTypeFont();
	this.font.loadFont("Scripts/data/futura.ttf",100);
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetColor(255);

	if (this.font)
		this.font.drawString("Murmur",w/2,h/2);
}

//--------------------------------------------------------------
function update(dt)
{
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	if (volume>=0.25)
	{
	}
}
