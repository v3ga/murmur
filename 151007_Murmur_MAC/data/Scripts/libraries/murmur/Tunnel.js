//--------------------------------------------------------------
function TunnelFrame(xO,yO)
{
	// A------B
	// |      |
	// |   O  |
	// |      |
	// D------C

	this.speed = 100;
	this.age = 0.0;
	this.ageMax = 4.0;
	this.rotation = 0.0;
	this.speedRotate = 30.0;
	this.colorGrey = 255;

	this.O = new fk.Vec3(xO,yO,0.0);
	this.A = new fk.Vec2();
	this.B = new fk.Vec2();
	this.C = new fk.Vec2();
	this.D = new fk.Vec2();
	
	this.rA = new fk.Vec2();
	this.rB = new fk.Vec2();
	this.rC = new fk.Vec2();
	this.rD = new fk.Vec2();

	this.setSpeed = function(s_){
		this.speed = s_;
	}

	this.setSpeedRotate = function(s_){
		this.speedRotate = s_;
	}

	this.setAgeMax = function(a_){
		this.ageMax = a_;
	}

	this.setA = function(x,y){
		this.A.set2(x-this.O.x,y-this.O.y);
	}
	this.setB = function(x,y){
		this.B.set2(x-this.O.x,y-this.O.y);
	}
	this.setC = function(x,y){
		this.C.set2(x-this.O.x,y-this.O.y);
	}
	this.setD = function(x,y){
		this.D.set2(x-this.O.x,y-this.O.y);
	}

	this.update = function(dt)
	{
		this.O.z -= this.speed*dt;
		this.age += dt;
		this.rotation += this.speedRotate*dt;
		this.colorGrey = 255.0*(1.0-this.age/this.ageMax);

		this.A.rotate( of.DegToRad(this.speedRotate*dt) );
		this.B.rotate( of.DegToRad(this.speedRotate*dt) );
		this.C.rotate( of.DegToRad(this.speedRotate*dt) );
		this.D.rotate( of.DegToRad(this.speedRotate*dt) );
	}

	this.draw = function()
	{
		of.SetColor(255,255,255,this.colorGrey);
		of.PushMatrix();
		of.Translate(this.O.x,this.O.y,this.O.z);
//		of.RotateZ(this.rotation);
		gl.Begin(GL_LINES);
			gl.Vertex2(this.A.x,this.A.y);
			gl.Vertex2(this.B.x,this.B.y);

			gl.Vertex2(this.B.x,this.B.y);
			gl.Vertex2(this.C.x,this.C.y);

			gl.Vertex2(this.C.x,this.C.y);
			gl.Vertex2(this.D.x,this.D.y);

			gl.Vertex2(this.D.x,this.D.y);
			gl.Vertex2(this.A.x,this.A.y);
		gl.End();
		of.PopMatrix();
	}
}

//--------------------------------------------------------------
function Tunnel()
{
	this.listFrames = new Array();
	this.isDrawConnections = 0;

	this.addFrame = function(frame_)
	{
		this.listFrames.push(frame_);
	}

	this.setSpeedRotate = function(s_)
	{
		var nb = this.listFrames.length;
		for (var i=0; i<nb; i++)
			this.listFrames[i].setSpeedRotate(s_);
	}
	
	this.setSpeed = function(s_)
	{
		var nb = this.listFrames.length;
		for (var i=0; i<nb; i++)
			this.listFrames[i].setSpeed(s_);
	
	}

	this.setAgeMax = function(a_)
	{
		var nb = this.listFrames.length;
		for (var i=0; i<nb; i++)
			this.listFrames[i].setAgeMax(a_);
	}
	
	this.setDrawConnections = function(is_)
	{
		this.isDrawConnections = is_;
	}
	
	this.update = function(dt)
	{
		var nb = this.listFrames.length;

		// Update
		for (var i=0; i<nb; i++)
			this.listFrames[i].update(dt);

		// Remove
		for (var i=nb-1; i>=0; i--)
		{
			if (this.listFrames[i].age >= this.listFrames[i].ageMax)
			{
				this.listFrames[i].age = this.listFrames[i].ageMax;
				this.listFrames.splice(i,1);
			}
		}
		
	}

	this.draw = function(w,h)
	{
		var nb = this.listFrames.length;
		
		for (var i=0; i<nb; i++)
			this.listFrames[i].draw();

		 // println("isDrawConnections="+this.isDrawConnections);

		  gl.Begin(GL_LINES);
		  
		  gl.Color4(1,1,1,0);gl.Vertex3(w/2,h/2,0);
		  gl.Color4(1,1,1,1);gl.Vertex3(0,0,0);

		  gl.Color4(1,1,1,0);gl.Vertex3(w/2,h/2,0);
		  gl.Color4(1,1,1,1);gl.Vertex3(w,0,0);

		  gl.Color4(1,1,1,0);gl.Vertex3(w/2,h/2,0);
		  gl.Color4(1,1,1,1);gl.Vertex3(w,h,0);

		  gl.Color4(1,1,1,0);gl.Vertex3(w/2,h/2,0);
		  gl.Color4(1,1,1,1);gl.Vertex3(0,h,0);

		  gl.End();
		
		if (this.isDrawConnections>0 && nb>=2)
		{
			

			for (var i=nb-1; i>=1; i--)
			{
				var frame0 = this.listFrames[i];
				var frame1 = this.listFrames[i-1];


				gl.Begin(GL_LINES);
				
				gl.Color4(1,1,1,frame0.colorGrey/255.0);gl.Vertex3(frame0.O.x+frame0.A.x,frame0.O.y+frame0.A.y, frame0.O.z);
				gl.Color4(1,1,1,frame1.colorGrey/255.0);gl.Vertex3(frame1.O.x+frame1.A.x,frame1.O.y+frame1.A.y, frame1.O.z);

				gl.Color4(1,1,1,frame0.colorGrey/255.0);gl.Vertex3(frame0.O.x+frame0.B.x,frame0.O.y+frame0.B.y, frame0.O.z);
				gl.Color4(1,1,1,frame1.colorGrey/255.0);gl.Vertex3(frame1.O.x+frame1.B.x,frame1.O.y+frame1.B.y, frame1.O.z);

				gl.Color4(1,1,1,frame0.colorGrey/255.0);gl.Vertex3(frame0.O.x+frame0.C.x,frame0.O.y+frame0.C.y, frame0.O.z);
				gl.Color4(1,1,1,frame1.colorGrey/255.0);gl.Vertex3(frame1.O.x+frame1.C.x,frame1.O.y+frame1.C.y, frame1.O.z);

				gl.Color4(1,1,1,frame0.colorGrey/255.0);gl.Vertex3(frame0.O.x+frame0.D.x,frame0.O.y+frame0.D.y, frame0.O.z);
				gl.Color4(1,1,1,frame1.colorGrey/255.0);gl.Vertex3(frame1.O.x+frame1.D.x,frame1.O.y+frame1.D.y, frame1.O.z);

				gl.End();
				
				
			}
		}

	}
}


