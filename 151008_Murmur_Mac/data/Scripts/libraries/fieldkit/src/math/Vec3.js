//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

fk.Vec3 = function() 
{
	function Vec3(_x, _y, _z) 
	{ 
		this.x = _x || 0;
		this.y = _y || 0;
		this.z = _z || 0;
	};

	Vec3.prototype.set = function(v) { this.x=v.x; this.y=v.y; this.z=v.z; return this; };
	Vec3.prototype.set3 = function(x,y,z) { this.x=x; this.y=y; this.z=z; return this; };
	Vec3.prototype.zero = function() { this.x=0; this.y=0; this.z=0; return this; };

	Vec3.prototype.add = function(v) { this.x += v.x; this.y += v.y; this.z += v.z; return this; };
	Vec3.prototype.add_ = function(v) { return new Vec3(this.x + v.x, this.y + v.y, this.z + v.z); };
	Vec3.prototype.add3 = function(x,y,z) { this.x += x; this.y += y; this.z += z; return this; };
		
	Vec3.prototype.sub = function(v) { this.x -= v.x; this.y -= v.y; this.z -= v.z; return this; };
	Vec3.prototype.sub_ = function(v) { return new Vec3(this.x - v.x, this.y - v.y, this.z - v.z); };
	Vec3.prototype.sub3 = function(x,y,z) { this.x -= x; this.y -= y; this.z -= z; return this; };
	    
	Vec3.prototype.mul = function(v) { this.x *= v.x; this.y *= v.y; this.z *= v.z; return this; };
	Vec3.prototype.mul_ = function(v) { return new Vec3(this.x * v.x, this.y * v.y, this.z * v.z); };
	Vec3.prototype.mul3 = function(x,y,z) { this.x *= x; this.y *= y; this.z *= z; return this; };
		
	Vec3.prototype.div = function(v) { this.x /= v.x; this.y /= v.y; this.z /= v.z; return this; };
	Vec3.prototype.div_ = function(v) { return new Vec3(this.x / v.x, this.y / v.y, this.z / v.z); };
	Vec3.prototype.div3 = function(x,y,z) { this.x /= x; this.y /= y; this.z /= z; return this; };
	
	Vec3.prototype.scale = function(value) { this.x *= value; this.y *= value; this.z *= value; return this; };
	Vec3.prototype.scale_ = function(value) { return new Vec3(this.x * value, this.y * value, this.z * value); };

	Vec3.prototype.length = function() { return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z); };
	Vec3.prototype.lengthSquared = function() { return this.x * this.x + this.y * this.y + this.z * this.z; };

	Vec3.prototype.normalize = function() 
	{
		var l = this.length();
		if(l != 0) {
			this.x /= l;
			this.y /= l;
			this.z /= l;
		}
		return this;
	};
	
	Vec3.prototype.normalizeTo = function(length)
	{
		var magnitude = Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
		if(magnitude > 0) {
			magnitude = length / magnitude;
			this.x *= magnitude;
			this.y *= magnitude;
			this.z *= magnitude;
		}
		return this;
	};

	Vec3.prototype.distance = function(v) { return Math.sqrt(this.distanceSquared3(v.x, v.y, v.z)); };
	Vec3.prototype.distanceSquared = function(v) { return this.distanceSquared3(v.x, v.y, v.z); };	
	Vec3.prototype.distanceSquared3 = function(x, y, z) 
	{ 
		var dx = this.x - x;
		var dy = this.y - y;
		var dz = this.z - z;
		return dx * dx + dy * dy + dz * dz;
	};


	Vec3.prototype.dot = function(v) { return this.x * v.x + this.y * v.y + this.z * v.z; };
	
	Vec3.prototype.jitter = function(amount) 
	{ 
		this.x += Random.float(-1,1) * amount;
		this.y += Random.float(-1,1) * amount;
		this.z += Random.float(-1,1) * amount;
		return this;
	};
	
	Vec3.prototype.jitter_ = function(amount) { return (new Vec3(this.x, this.y, this.z)).jitter(amount); };

	Vec3.prototype.lerp = function(target, delta)
	{
		this.x = this.x * (1 - delta) + target.x * delta;
		this.y = this.y * (1 - delta) + target.y * delta;
		this.z = this.z * (1 - delta) + target.z * delta;
		return this;
	};
	
	Vec3.prototype.lerp_ = function(target, delta)
	{
		return (new Vec3(this.x, this.y, this.z)).lerp(target, delta);
	};
	
	Vec3.prototype.equals = function(v)	
	{
		return this.x == v.x && this.y == v.y && this.z == v.z;
	};
	
	Vec3.prototype.clone = function()
	{
		return new Vec3(this.x, this.y, this.z);
	};

	Vec3.prototype.toString = function() { return "Vec3["+ this.x +","+ this.y +","+ this.z +"]"; };
	
	return Vec3;
}();

