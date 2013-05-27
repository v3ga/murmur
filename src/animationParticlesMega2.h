//
//  animationParticlesMega2.h
//  murmur
//
//  Created by Julien on 17/05/13.
//
//

#pragma once

#include "animations.h"

class Particle
{
public:
	float x, y;
	float xv, yv;
	float xf, yf;
	Particle(float _x = 0, float _y = 0,
             float _xv = 0, float _yv = 0) :
    x(_x), y(_y),
    xv(_xv), yv(_yv) {
	}
	void updatePosition(float timeStep) {
		// f = ma, m = 1, f = a, v = int(a)
		xv += xf;
		yv += yf;
		x += xv * timeStep;
		y += yv * timeStep;
	}
	void resetForce() {
		xf = 0;
		yf = 0;
	}
	void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3) {
		bool collision = false;
        
		if (x > right){
			x = right;
			xv *= -1;
			collision = true;
		} else if (x < left){
			x = left;
			xv *= -1;
			collision = true;
		}
        
		if (y > bottom){
			y = bottom;
			yv *= -1;
			collision = true;
		} else if (y < top){
			y = top;
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


#define DRAW_FORCES
#define USE_INVSQRT

class ParticleSystem {
protected:
	float timeStep;
	vector<Particle> particles;
	vector< vector<Particle*> > bins;
	int width, height, k, xBins, yBins, binSize;
    
public:
	ParticleSystem();
    
	void setup(int width, int height, int k);
	void setTimeStep(float timeStep);
    
	void add(Particle particle);
	vector<Particle*> getNeighbors(Particle& particle, float radius);
	vector<Particle*> getNeighbors(float x, float y, float radius);
	vector<Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
	unsigned size() const;
	Particle& operator[](unsigned i);
    
	void setupForces();
	void addRepulsionForce(const Particle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float radius, float scale);
	void addAttractionForce(const Particle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	void addForce(const Particle& particle, float radius, float scale);
	void addForce(float x, float y, float radius, float scale);
	void update();
    
	void draw();
};

inline float InvSqrt(float x){
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}


class AnimationParticlesMega2 : public Animation
{
public:
    
    AnimationParticlesMega2(string name);
    
    virtual	void			VM_enter				();
    virtual void			VM_update				(float dt);
    virtual void			VM_draw					(float w, float h);
    virtual	void			VM_exit					();
    virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);

    virtual void            createUICustom          ();
    virtual void            guiEvent                (ofxUIEventArgs &e);

    
    void                    createParticles         ();
    
    
private:
    ofVec2f                 m_anchor;
    ofVec2f                 m_resolution;
    bool                    m_isParticlesInit;
    float                   m_volume, m_volumeTarget;
    
    float                   timeStep;
	int                     lineOpacity, pointOpacity;
	float                   particleNeighborhood, particleRepulsion;
	float                   centerAttraction;
    
	int                     kParticles;
	ParticleSystem          particleSystem;
	bool                    isMousePressed, slowMotion;
    
    
    float                   m_repulsionRadius;
    float                   m_ampRepulsion;
    float                   m_ampAttraction;
};
