//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

fk.Vec2 = function() {
	function Vec2(_x, _y) 
	{ 
		this.x = _x || 0;
		this.y = _y || 0;
	};

	Vec2.prototype.set2 = function(x,y) { this.x = x; this.y = y; return this; };
	Vec2.prototype.set = function(v) { this.x = v.x; this.y = v.y; return this; };
	Vec2.prototype.zero = function() { this.x = 0; this.y = 0; return this; };

	Vec2.prototype.add = function(v) { this.x += v.x; this.y += v.y; return this; };
	Vec2.prototype.add_ = function(v) { return new Vec2(this.x + v.x, this.y + v.y); };
	
	Vec2.prototype.sub = function(v) { this.x -= v.x; this.y -= v.y; return this; };
	Vec2.prototype.sub_ = function(v) { return new Vec2(this.x - v.x, this.y - v.y); };
    
	Vec2.prototype.mul = function(v) { this.x *= v.x; this.y *= v.y; return this; };
	Vec2.prototype.mul_ = function(v) { return new Vec2(this.x * v.x, this.y * v.y); };
	
	Vec2.prototype.div = function(v) { this.x /= v.x; this.y /= v.y; return this; };
	Vec2.prototype.div_ = function(v) { return new Vec2(this.x / v.x, this.y / v.y); };

	Vec2.prototype.scale = function(value) { this.x *= value; this.y *= value; return this; };
	Vec2.prototype.scale_ = function(value) { return new Vec2(this.x * value, this.y * value); };

	Vec2.prototype.length = function() { return Math.sqrt(this.x * this.x + this.y * this.y); };
	Vec2.prototype.lengthSquared = function() { return this.x * this.x + this.y * this.y; };

    // taken from https://github.com/processing/processing/blob/master/core/src/processing/core/PVector.java
    Vec2.prototype.limit = function(max)
    {
        if (this.lengthSquared()>max*max)
        {
            this.normalize();
            this.scale(max);
        }
    }
    
	Vec2.prototype.normalize = function() 
	{
		var l = this.length();
		if(l != 0) {
			this.x /= l;
			this.y /= l;
		}
		return this;
	};
	
	Vec2.prototype.normalizeTo = function(length)
	{
		var magnitude = Math.sqrt(this.x * this.x + this.y * this.y);
		if(magnitude > 0) {
			magnitude = length / magnitude;
			this.x *= magnitude;
			this.y *= magnitude;
		}
		return this;
	};

	Vec2.prototype.distance = function(v) { return Math.sqrt(this.distanceSquared2(v.x, v.y)); };
	Vec2.prototype.distanceSquared = function(v) { return this.distanceSquared2(v.x, v.y); };	
	Vec2.prototype.distanceSquared2 = function(x, y) 
	{ 
		var dx = this.x - x;
		var dy = this.y - y;
		return dx * dx + dy * dy;
	};


	Vec2.prototype.dot = function(v) { return this.x * v.x + this.y * v.y; };
	
	Vec2.prototype.rotate = function(angle)
	{
		var sina = Math.sin(angle);
		var cosa = Math.cos(angle);
		var rx = this.x * cosa - this.y * sina;
		var ry = this.x * sina + this.y * cosa;
		this.x = rx;
		this.y = ry;	
	};

	Vec2.prototype.jitter = function(amount) 
	{ 
		this.x += Random.float(-1,1) * amount;
		this.y += Random.float(-1,1) * amount;
		return this;
	};
	
	Vec2.prototype.jitter_ = function(amount) { return (new Vec2(this.x, this.y)).jitter(amount); };

	Vec2.prototype.lerp = function(target, delta)
	{
		this.x = this.x * (1 - delta) + target.x * delta;
		this.y = this.y * (1 - delta) + target.y * delta;
		return this;
	};
	
	Vec2.prototype.lerp_ = function(target, delta)
	{
		return (new Vec2(this.x, this.y)).lerp(target, delta);
	};
	
	Vec2.prototype.equals = function(v)	
	{
		return this.x == v.x && this.y == v.y;
	};
	
	Vec2.prototype.clone = function()
	{
		return new Vec2(this.x, this.y);
	};

	Vec2.prototype.toString = function() { return "Vec2["+ this.x +","+ this.y +"]"; };
	
	return Vec2;
}();
