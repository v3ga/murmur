//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.


fk.Rect = function() {
	var Rect = function(_x1, _y1, _x2, _y2) 
	{
		this.x1 = _x1 || 0;
		this.y1 = _y1 || 0;
		this.x2 = _x2 || 0;
		this.y2 = _y2 || 0;
	};
	
	Rect.prototype.set = function(_x1, _y1, _x2, _y2) 
	{
		this.x1 = _x1 || 0;
		this.y1 = _y1 || 0;
		this.x2 = _x2 || 0;
		this.y2 = _y2 || 0;
	};
	
	Rect.prototype.reset = function()
	{
		this.x1 = this.y1 = Number.MAX_VALUE;
		this.x2 = this.y2 = Number.MIN_VALUE;
	};
	
	Rect.prototype.fit = function(p)
	{
		if(p.x < this.x1) this.x1 = p.x;
		if(p.y < this.y1) this.y1 = p.y;
		if(p.x > this.x2) this.x2 = p.x;
		if(p.y > this.y2) this.y2 = p.y;
	}
	
	Rect.prototype.contains = function(p)
	{
		return p.x > this.x1 && p.x < this.x2 &&
			   p.y > this.y1 && p.y < this.y2;
	};
	
	Rect.prototype.contains2 = function(x,y)
	{
		return x > this.x1 && x < this.x2 &&
			   y > this.y1 && y < this.y2;
	};
	
	Rect.prototype.width = function() { return this.x2 - this.x1; };
	Rect.prototype.height = function() { return this.y2 - this.y1; };
	
	Rect.prototype.clone = function()
	{
		return new Rect(this.x1, this.y1, this.x2, this.y2);
	};

	Rect.prototype.toString = function() { return "Rect["+ this.x1 +","+ this.y1 +" "+ this.width() +","+ this.height() +"]"; };
	
	return Rect;
}();

