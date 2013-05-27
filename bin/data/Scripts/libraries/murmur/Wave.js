function Wave(x,y, nbPoints)
{
	this.loc = new fk.Vec2(x,y);
	this.dir = new fk.Vec2(0,0);
	this.radius = 0.0;
	this.radiusSpeed = 50.0;

	this.points = Array();
	this.step = 2*Math.PI / nbPoints;

	this.phase = 0;
	this.age = 0;
	this.ageMax = 10.0;


	//--------------------------------------------------------------
	for (var i=0;i<nbPoints;i++)
		this.points.push( new fk.Vec2(0,0) );

    
	//--------------------------------------------------------------
    this.setDir = function(dirx, diry){this.dir.set2(dirx,diry);}
    
	//--------------------------------------------------------------
	this.setAgeMax = function(ageMax){this.ageMax = ageMax;}

	//--------------------------------------------------------------
	this.setSpeed = function(s){this.radiusSpeed = s;}

	//--------------------------------------------------------------
	this.setPhase = function(p){this.phase = p;}

	//--------------------------------------------------------------
	this.isOld = function(){ return (this.age>=this.ageMax); }

	//--------------------------------------------------------------
	this.getAlpha = function(){ return Math.max(0, 1.0-this.age/this.ageMax); }

	//--------------------------------------------------------------
	this.update = function(dt)
	{
		this.age += dt;
		this.radius += this.radiusSpeed*dt;
		this.updatePoints();
	}

	//--------------------------------------------------------------
	this.updatePoints = function()
	{
        this.loc.x += this.dir.x;
        this.loc.y += this.dir.y;
        
		var p;
		var angle=0;
		for (var i=0;i<this.points.length;i++)
		{
			p = this.points[i];
			p.set2(this.loc.x+this.radius*Math.cos(this.phase+angle), this.loc.y+this.radius*Math.sin(this.phase+angle));
			angle+=this.step;
		}
	}

	//--------------------------------------------------------------
	this.draw = function()
	{
		var p;
		gl.Begin(GL_LINE_LOOP);
		for (var i=0;i<this.points.length;i++)
		{
			p = this.points[i];
			gl.Vertex2(p.x, p.y);
		}
		gl.End();
	}

}
