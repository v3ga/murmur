this.angle = 0;
this.volume = 0;
this.x = 0;
this.y = 0;
this.d = 0;
this.i = 0;

this.cx = 0;
this.cy = 0;
this.r = 100;
this.num_segments = 100;

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
	of.SetColor(255);
	of.SetLineWidth(100);

	gl.Begin(GL_LINE_LOOP); 

	var theta,xx,yy;
	for(var ii = 0; ii < this.num_segments; ii++) 
	{ 
		theta = 2.0 * 3.1415926 * ii / num_segments;//get the current angle 

		xx = w/2+this.r * Math.cos(theta);//calculate the x component 
		yy = h/2+this.r * Math.sin(theta);//calculate the y component 

		gl.Vertex2(xx + cx, yy + cy);//output vertex 

	} 


	gl.End(); 


	of.SetLineWidth(1);

}


//----- appeler juste avant de dessiner | le dt (temps depuis le dernière update) ---------------------------------------------------------
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
