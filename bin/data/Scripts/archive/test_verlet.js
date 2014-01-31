
this.sim = null;

//--------------------------------------------------------------
function getTheme(){ return THEME_CREATURE; }

//--------------------------------------------------------------
function setup()
{
}

//--------------------------------------------------------------
function draw(w,h)
{
	if (this.sim == null){

		this.sim = new VerletJS(w,h);

		this.tire1 = this.sim.tire(new Vec2(200,50), 50, 30, 0.3, 0.9);
		this.tire2 = this.sim.tire(new Vec2(400,50), 70, 7, 0.1, 0.2);
		this.tire3 = this.sim.tire(new Vec2(600,50), 70, 3, 1, 1);
	}

	this.sim.frame(16);

	of.Background(0,0,0);	
	of.SetLineWidth(3);
	this.sim.draw();
	of.SetLineWidth(1);
}

//--------------------------------------------------------------
function update(dt)
{
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
}
