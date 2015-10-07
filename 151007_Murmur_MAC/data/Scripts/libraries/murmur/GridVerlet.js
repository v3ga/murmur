VerletJS.prototype.grid = function(width, height, nbPoints, stiffness) {
	
	var composite = new this.Composite();
	
	var xStride = width/(nbPoints-1);
	var yStride = height/(nbPoints-1);
	
	var origin = new Vec2(0,0);

	var x,y;
	for (y=0;y<nbPoints;++y) {
		for (x=0;x<nbPoints;++x) {
			var px = origin.x + x*xStride;// + xStride/2;
			var py = origin.y + y*yStride;  //+ yStride/2;
			composite.particles.push(new Particle(new Vec2(px, py)));
			
			if (x > 0)
				composite.constraints.push(new DistanceConstraint(composite.particles[y*nbPoints+x], composite.particles[y*nbPoints+x-1], stiffness));
			
			if (y > 0)
				composite.constraints.push(new DistanceConstraint(composite.particles[y*nbPoints+x], composite.particles[(y-1)*nbPoints+x], stiffness));
		}
	}
	

	composite.pin(0);
	composite.pin(nbPoints-1);
	composite.pin(nbPoints*(nbPoints-1));
	composite.pin(nbPoints-1+nbPoints*(nbPoints-1));


	this.composites.push(composite);
	return composite;
}

/*
function GridVerlet(nbPointsX, nbPointsY)
{
	this.nbPointX = nbPointsX;
	this.nbPointY = nbPointsY;

	this.create = function(w,h)
	{
		this.nodes = Array();
		var stepX = w/(nbPointX-1);
		var stepY = h/(nbPointY-1);
		var x=0,y=0;
		var i,j,offset;
		for (j=0;j<nbPointY;j++)
		{
			for (i=0;i<nbPointX;i++)
			{
				offset = i+j*this.nbPointX;
			}
		}


	}



}
*/
