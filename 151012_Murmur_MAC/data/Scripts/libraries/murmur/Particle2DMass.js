function Particle2DMass(x,y,size,gravity,mass)
{

	// ------------------------------------------
	this.loc = new fk.Vec2(x,y);	
	this.vel = new fk.Vec2();
	this.acc = new fk.Vec2();
	this.sz = size;
	this.gravity = gravity;
	this.mass = mass;
	this.d = 0;
	this.dToPoint = 0;
 	this.velocityLimit = 2.5;
 
	this.targetLoc = new fk.Vec2();
	this.dir = new fk.Vec2();
	this.force = 0;

	this.locPointRef = new fk.Vec2();
	this.dRepulsion = 180;

	// ------------------------------------------
	this.setDiamRepulsion = function(d)
	{
		this.dRepulsion = d;
	}

	// ------------------------------------------
	this.setPointRef = function(x,y)
	{
		this.locPointRef.set2(x,y);
	}

	// ------------------------------------------
	this.forces = function()
	{
		this.targetLoc.set(this.locPointRef);

		this.dir.set(this.loc);
		this.dir.sub(this.targetLoc);
		this.dir.normalize();

		this.force = (this.gravity*this.mass)/(this.d*this.d);
	    this.dToPoint = of.Dist(this.loc.x,this.loc.y,this.locPointRef.x,this.locPointRef.y);
	    if (this.dToPoint<=0.5*this.dRepulsion)
		{
			this.dir.scale(2.25);
		}
		else{
			this.dir.scale(-1);
		}

		this.applyForce(this.dir);
	}

	// ------------------------------------------
	this.applyForce = function(f)
	{
	    f.scale(1/this.mass);
	    this.acc.add(f);
	}

	// ------------------------------------------
	this.display = function()
	{
		of.Ellipse(this.loc.x,this.loc.y,this.sz,this.sz);
	}

	// ------------------------------------------
	this.update = function()
	{
		this.vel.add(this.acc);
        this.vel.limit(this.velocityLimit);
		this.loc.add(this.vel);
		this.acc.scale(0);
	}	

	// ------------------------------------------
	this.run = function()
	{
		this.forces();
		this.display();
		this.update();
	}

}