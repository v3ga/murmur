//--------------------------------------------------------------
// Configuration
this.gridNbPoints = 20;

//--------------------------------------------------------------
this.volume = 0;
this.xAcc = 0;
this.yAcc = 0;
this.width = 0;
this.height = 0;

//--------------------------------------------------------------
function getTheme(){ return THEME_CREATURE; }

//--------------------------------------------------------------
function createGrid(w,h)
{
	if (this.grid==null)
	{
		this.grid = new Grid3D(w,h,this.gridNbPoints);	
	}
}

//--------------------------------------------------------------
function setup()
{
	this.grid = null;
}

//--------------------------------------------------------------
function draw(w,h)
{
	createGrid(w,h);

	of.Background(0,0,0);
//	of.SetColor(0,0,0,100);
//	of.Rect(0,0,w,h);
	of.SetColor(255,255,255,255);
	of.SetLineWidth(3);
	if (this.grid){
		this.grid.deform(this.xAcc,this.yAcc, Math.min(w,h)*this.volume*0.55);		
		this.grid.draw();
	}
	of.SetLineWidth(1);
}

//--------------------------------------------------------------
function update(dt)
{
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	this.volume = volume;
	this.xAcc = x;
	this.yAcc = y;
}
