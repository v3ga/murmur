//
//  Particle.h
//  murmur
//
//  Created by Julien on 02/10/2015.
//
//

#pragma once
#include "ofMain.h"

class Particle
{
public:
	ofVec3f* mp_pos;
	ofFloatColor* mp_color;
	int m_index;
	float x, y;
	float xv, yv;
	float xf, yf;
	float mass;
	Particle(float _x = 0, float _y = 0, float _xv = 0, float _yv = 0) :
    x(_x), y(_y),
    xv(_xv), yv(_yv) {
	}

	Particle(ofVec3f* pPos,ofFloatColor* pColor, float _xv = 0, float _yv = 0) :
    xv(_xv), yv(_yv)
	{
		mp_pos = pPos;
		mp_color = pColor;
	}
	
	Particle(ofVec3f* pPos, float _xv = 0, float _yv = 0) :
    xv(_xv), yv(_yv)
	{
		mp_pos = pPos;
	}

	void updatePosition(float timeStep) {
		// f = ma, m = 1, f = a, v = int(a)
		xv += xf;
		yv += yf;
		
		xv = ofClamp(xv,-5,5);
		yv = ofClamp(yv,-5,5);
//		x += xv * timeStep;
//		y += yv * timeStep;
		mp_pos->x += xv * timeStep;
		mp_pos->y += yv * timeStep;
	}
	void resetForce() {
		xf = 0;
		yf = 0;
	}
	void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3) {
		bool collision = false;
        
//		if (x > right){
		if (mp_pos->x > right){
			mp_pos->x = right;//x = right;
			xv *= -1;
			collision = true;
//		} else if (x < left){
		} else if (mp_pos->x < left){
//			x = left;
			mp_pos->x = left;
			xv *= -1;
			collision = true;
		}
        
//		if (y > bottom){
		if (mp_pos->y > bottom){
//			y = bottom;
			mp_pos->y = bottom;
			yv *= -1;
			collision = true;
//		} else if (y < top){
		} else if (mp_pos->y < top){
//			y = top;
			mp_pos->y = top;
			yv *= -1;
			collision = true;
		}
        
		if (collision == true){
			xv *= damping;
			yv *= damping;
		}
	}
	void addDampingForce(float damping = .01) {
		xf = xf - xv * damping;
        yf = yf - yv * damping;
	}
	void draw() {
		glVertex2f(x, y);
	}
};