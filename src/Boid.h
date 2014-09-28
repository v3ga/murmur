//
//  Flock.h
//  murmur
//
//  Created by Julien on 25/09/2014.
//
//
// http://processing.org/examples/flocking.html
//

#pragma once
#include "ofMain.h"

class Boid
{

	public:
	Boid					(float x, float y);
	
	virtual void			run				(vector<Boid*>& boids);
	virtual	void			draw			(){}

	virtual void 			applyForce		(ofVec2f force);
	virtual void 			flock			(vector<Boid*>& boids);
	virtual void			update			(float dt);
	virtual ofVec2f 		seek			(ofVec2f target);
	virtual ofVec2f 		separate 		(vector<Boid*>& boids);
	virtual ofVec2f 		align 			(vector<Boid*>& boids);
  	virtual ofVec2f 		cohesion 		(vector<Boid*>& boids);



	ofVec2f 		location;
	ofVec2f 		velocity;
	ofVec2f 		acceleration;

	float 			r;
	float 			maxforce;    // Maximum steering force
	float 			maxspeed;    // Maximum speed
};


/*  void render() {
    // Draw a triangle rotated in the direction of velocity
    float theta = velocity.heading2D() + radians(90);
    // heading2D() above is now heading() but leaving old syntax until Processing.js catches up
    
    fill(200, 100);
    stroke(255);
    pushMatrix();
    translate(location.x, location.y);
    rotate(theta);
    beginShape(TRIANGLES);
    vertex(0, -r*2);
    vertex(-r, r*2);
    vertex(r, r*2);
    endShape();
    popMatrix();
  }
*/




