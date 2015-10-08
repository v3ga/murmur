
this.sim = null;

//--------------------------------------------------------------
function getTheme(){ return THEME_CREATURE; }

//--------------------------------------------------------------
function setup()
{
}

//--------------------------------------------------------------
function createSim(w,h)
{
	if (this.sim == null){
		this.sim = new VerletJS(w,h);
		this.sim.gravity = new Vec2(0,0);
		this.sim.bounds = function(p){}
		this.grid = this.sim.grid(w,h, 11, 0.7);
	}
}

//--------------------------------------------------------------
function draw(w,h)
{
	createSim(w,h);

	this.sim.frame(10);

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
