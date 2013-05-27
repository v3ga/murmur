//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

(function() {

	fk.math = fk.math || {};
	var ns = fk.math;

	// Constants
	ns.PI = Math.PI;
	ns.TWO_PI = Math.PI * 2.0;
	ns.HALF_PI = Math.PI * 0.5;
	ns.EPSILON = 0.001;
	ns.DEG_TO_RAD = Math.PI / 180;
	ns.RAD_TO_DEG = 180 / Math.PI;
	
	// Utilities
	ns.min = function(a, b) {
	  if (a < b) return a;
	  return b;
	};

	ns.max = function(a, b) {
	  if (a > b) return a;
	  return b;
	};

	// Keep the value |v| in the range vmin .. vmax.  This matches GLSL clamp().
	ns.clamp = function(v, vmin, vmax) {
	  return ns.min(vmax, ns.max(vmin, v));
	};
	
	ns.lerp = function(current, target, delta)
	{
		return current * (1 - delta) + target * delta;
	};

	ns.round = function(value, dec) 
	{
		var exp = Math.pow(10,dec);
		return Math.floor(value * exp) / exp;
	};
	
}()); 
