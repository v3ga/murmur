//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

/**
 * A simple line from multiple segments, can be sampled at any point.
 * NOTE this curve does not always run through all given points.
 * Work with any object type as long as it has x and y number properties.
 */
fk.Polyline = function(_points)
{
	var points = _points || [];

	this.add = function(point) { points.push(point); };
	this.points = function() { return points; };
    this.size = function() { return points.length; };
    this.clear = function() { points = []; };

    this.point = function(time)
    {
        // first point
        if(time <= 0) {
            return points[0];

        // last point
        } else if (time >= 1) {
            return points[points.length - 1];

        // in between
        } else {
            var median = time * (points.length - 1);
            var prev = points[Math.floor(median)];
            var next = points[Math.ceil(median)];

            return {
                x: prev.x + (next.x - prev.x) * 0.5,
                y: prev.y + (next.y - prev.y) * 0.5
            };
        }
    };

	// operations
	this.scale = function(value)
	{
		for(var i=points.length; i--;) {
			var p = points[i];
			p.x *= value;
			p.y *= value;
		}
	};

	this.translate = function(x, y)
	{
		for(var i=points.length; i--;) {
			var p = points[i];
			p.x += x;
			p.y += y;
		}
	};

	this.rotate = function(angle)
	{
		var sina = Math.sin(angle);
		var cosa = Math.cos(angle);

		for(var i=points.length; i--;) {			
			var p = points[i];
			var rx = p.x * cosa - p.y * sina;
			var ry = p.x * sina + p.y * cosa;
			p.x = rx;
			p.y = ry;
		}
	};

	// accessors
	this.width = function()
	{
		var min = Number.MAX_VALUE;
		var max = Number.MIN_VALUE;
		for(var i=points.length; i--;) {
			var p = points[i];
			if(p.x < min) min = p.x;
			if(p.x > max) max = p.x;
		}
		return max-min;
	};

	this.height = function()
	{
		var min = Number.MAX_VALUE;
		var max = Number.MIN_VALUE;
		for(var i=points.length; i--;) {
			var p = points[i];
			if(p.y < min) min = p.y;
			if(p.y > max) max = p.y;
		}
		return max-min;
	};
};