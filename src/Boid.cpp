//
//  Flock.cpp
//  murmur
//
//  Created by Julien on 25/09/2014.
//
//

#include "Boid.h"


//--------------------------------------------------------------
Boid::Boid(float x, float y)
{
	acceleration.set(0.0f,0.0f);

	// Leaving the code temporarily this way so that this example runs in JS
	float angle = ofRandom(TWO_PI);
	velocity.set(cos(angle), sin(angle));
	location.set(x, y);

	r = 2.0;
	maxspeed = 2;
	maxforce = 0.03;
}

//--------------------------------------------------------------
void Boid::run(vector<Boid*>& boids)
{
    flock(boids);
    update(0.0f);
    //render();
}

//--------------------------------------------------------------
void Boid::applyForce(ofVec2f force)
{
    // We could add mass here if we want A = F / M
    acceleration += force;
}

//--------------------------------------------------------------
// We accumulate a new acceleration each time based on three rules
void Boid::flock(vector<Boid*>& boids)
{
	ofVec2f sep = separate(boids);   // Separation
	ofVec2f ali = align(boids);      // Alignment
	ofVec2f coh = cohesion(boids);   // Cohesion
 
	// Arbitrarily weight these forces
	sep *= 1.5;
	ali *= 1.0;
	coh *= 1.0;

	// Add the force vectors to acceleration
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}


//--------------------------------------------------------------
// Method to update location
void Boid::update(float dt)
{
	 // Update velocity
 	velocity+=acceleration;
 	// Limit speed
 	velocity.limit(maxspeed);
 	location+=velocity;
 	// Reset accelertion to 0 each cycle
 	acceleration.set(0.0f,0.0f);
}

//--------------------------------------------------------------
// A method that calculates and applies a steering force towards a target
// STEER = DESIRED MINUS VELOCITY
ofVec2f Boid::seek(ofVec2f target)
{
	ofVec2f desired = target - location;  // A vector pointing from the location to the target

	// Scale to maximum speed
	desired.normalize();
	desired*=maxspeed;

	// Above two lines of code below could be condensed with new ofVec2f setMag() method
	// Not using this method until Processing.js catches up
	// desired.setMag(maxspeed);

	// Steering = Desired minus Velocity
	ofVec2f steer = desired - velocity;
	steer.limit(maxforce);  // Limit to maximum steering force
	return steer;
}

//--------------------------------------------------------------
// Separation
// Method checks for nearby boids and steers away
ofVec2f Boid::separate (vector<Boid*>& boids)
{
	float desiredseparation = 25.0f;
	ofVec2f steer(0.0f,0.0f);
    int count = 0;
    // For every boid in the system, check if it's too close
	vector<Boid*>::iterator it = boids.begin();
    for ( ; it != boids.end(); ++it)
	{
		Boid* other = *it;

		float d = location.distance(other->location);

		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if ((d > 0) && (d < desiredseparation))
		{
        	// Calculate vector pointing away from neighbor
        	ofVec2f diff = location - other->location;
	        diff.normalize();
    	    diff/=d;        // Weight by distance
        	steer+=diff;
        	count++;            // Keep track of how many
      	}
    }
    // Average -- divide by how many
    if (count > 0)
	{
      steer /= (float)count;
    }

    // As long as the vector is greater than 0
    if (steer.length() > 0)
	{
      // First two lines of code below could be condensed with new ofVec2f setMag() method
      // Not using this method until Processing.js catches up
      // steer.setMag(maxspeed);

      // Implement Reynolds: Steering = Desired - Velocity
      steer.normalize();
      steer*=maxspeed;
      steer = steer-velocity;
      steer.limit(maxforce);
    }
    return steer;
}


//--------------------------------------------------------------
// Alignment
// For every nearby boid in the system, calculate the average velocity
ofVec2f Boid::align(vector<Boid*>& boids)
{
    float neighbordist = 50;
    ofVec2f sum(0.0f, 0.0f);
    int count = 0;
	vector<Boid*>::iterator it = boids.begin();
    for ( ; it != boids.end(); ++it)
	{
		Boid* other = *it;
		float d = location.distance(other->location);
     
	  	if ((d > 0) && (d < neighbordist))
		{
        	sum+=other->velocity;
        	count++;
      	}
    }
	
    if (count > 0)
	{
      sum/=(float)count;
      // First two lines of code below could be condensed with new ofVec2f setMag() method
      // Not using this method until Processing.js catches up
      // sum.setMag(maxspeed);

      // Implement Reynolds: Steering = Desired - Velocity
      sum.normalize();
      sum*=maxspeed;
      ofVec2f steer = sum-velocity;
      steer.limit(maxforce);
      return steer;
    } 
    else
	{
      return ofVec2f(0, 0);
    }
}


//--------------------------------------------------------------
// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
ofVec2f Boid::cohesion(vector<Boid*>& boids)
{
	 float neighbordist = 50;
	 ofVec2f sum(0, 0);   // Start with empty vector to accumulate all locations
	 int count = 0;
	 vector<Boid*>::iterator it = boids.begin();
	 for ( ; it != boids.end(); ++it)
	 {
		Boid* other = *it;
		 float d = location.distance(other->location);
		 if ((d > 0) && (d < neighbordist))
		 {
	 		sum+=other->location; // Add location
	 		count++;
		 }
	 }

	 if (count > 0)
	 {
	 	sum/=count;
	 	return seek(sum);  // Steer towards the location
	 } 
	 else
	 {
	 	return ofVec2f(0, 0);
	 }
}

