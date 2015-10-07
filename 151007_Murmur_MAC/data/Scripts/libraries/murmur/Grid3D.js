function Grid3D(width, height, nbPoints) 
{
	var xStride = width/(nbPoints-1);
	var yStride = height/(nbPoints-1);
	
	var origin = new Vec2(0,0);

	this.points = Array();
	this.triangles = Array();

	var x,y;
	for (y=0;y<nbPoints;++y) 
	{
		for (x=0;x<nbPoints;++x) 
		{
			var px = origin.x + x*xStride;
			var py = origin.y + y*yStride;
			var pz = 0;

			this.points.push( new fk.Vec3(px,py,pz) );
		}
	}

	var pA,pB,pC,pB;
	for (y=0;y<nbPoints-1;++y) 
	{
		for (x=0;x<nbPoints-1;++x) 
		{
			pA = this.points[x  +nbPoints*y];
			pB = this.points[x+1+nbPoints*y];
			pC = this.points[x+1+nbPoints*(y+1)];
			pD = this.points[x  +nbPoints*(y+1)];

			this.triangles.push( {A:pA, B:pC, C:pD} );
//			this.triangles.push( {A:pA, B:pB, C:pC} );
		}
	}

	this.deform = function(xc,yc,radius)
	{
		var d = 0;
		var p;
		for (var i=0; i<this.points.length; i++)
		{
			p = this.points[i];
			d = of.Dist(xc,yc,p.x,p.y);
			if (d<=radius){
				p.z = 300 * (1.0-d/radius);
			}
			else
				p.z = 0;
		}
	}

	this.update = function()
	{


	}

	this.draw = function()
	{
/*		var p;
		for (var i=0;i<this.points.length;i++)
		{
			p = this.points[i];
			of.PushMatrix();
			of.Translate(p.x, p.y, p.z);
			of.Ellipse(0,0,5,5);			
			of.PopMatrix();
		}
*/

		var t;
		gl.Begin(GL_LINES);
		for (var i=0;i<this.triangles.length;i++)
		{
			t = this.triangles[i];
			gl.Vertex3(t.A.x,t.A.y,t.A.z);
			gl.Vertex3(t.B.x,t.B.y,t.B.z);
		}
		gl.End();

/*
		gl.Begin(GL_TRIANGLES);
		var t;
		for (var i=0;i<this.triangles.length;i++)
		{
			if (i%2==0){
				t = this.triangles[i];
				gl.Vertex3(t.A.x,t.A.y,t.A.z);
				gl.Vertex3(t.B.x,t.B.y,t.B.z);
				gl.Vertex3(t.C.x,t.C.y,t.C.z);
			}
		}
		gl.End();
*/

	}
}
