//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

/** 
* A Catmull-Rom spline (which is a special form of the cubic hermite curve) implementation, 
* generates a smooth curve/interpolation from a number of Vec2 points.
*/
fk.Spline = function(_points)
{
	var points = _points || [];
	
	this.add = function(point)
	{
		points.push(point);
	};
	
	this.points = function() { return points; };
    this.size = function() { return points.length; };
    this.clear = function() { points = []; };

	var clone = function(p) {
		return new fk.Vec2(p.x, p.y);
	};
	
    this.point = function(time)
    {			
		if(points.length < 4) return;

        // first point
        if (time <= 0) {
            return points[0];

        // last point
        } else if (time >= 1) {
            return points[points.length - 1];

        // in between
        } else {
            var size = points.length;
            var partPercentage = 1.0 / (size - 1);
            var timeBetween = time / partPercentage;

            var i = Math.floor(timeBetween);
            var normalizedTime = timeBetween - i;

            var t = normalizedTime * 0.5;
            var t2 = t * normalizedTime;
            var t3 = t2 * normalizedTime;

			var tmp1;
            if(--i == -1) {
				tmp1 = clone(points[0]);
            } else {
				tmp1 = clone(points[i]);
            }

            var tmp2 = clone(points[++i]);
            var tmp3 = clone(points[++i]);

			var tmp4;
            if(++i == size) {
				tmp4 = clone(points[points.length-1]);
            } else {
				tmp4 = clone(points[i]);
            }

            // calculate point
			tmp1.scale(-t3 + 2 * t2 - t);
			var result = tmp1;

			tmp2.scale(3 * t3 - 5 * t2 + 1);
			result.add(tmp2);

			tmp3.scale(-3 * t3 + 4 * t2 + t);
			result.add(tmp3);

			tmp4.scale(t3 - t2);
			result.add(tmp4);
			
            return result;
        }
    };
};
