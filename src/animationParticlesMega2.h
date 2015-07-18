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
	ofVec3f* mp_pos;
	ofFloatColor* mp_color;
	int m_index;
	float x, y;
	float xv, yv;
	float xf, yf;
	Particle(float _x = 0, float _y = 0,
             float _xv = 0, float _yv = 0) :
    x(_x), y(_y),
    xv(_xv), yv(_yv) {
	}

	Particle(ofVec3f* pPos,ofFloatColor* pColor,
             float _xv = 0, float _yv = 0) :
    xv(_xv), yv(_yv)
	{
		mp_pos = pPos;
		mp_color = pColor;
	}

	void updatePosition(float timeStep) {
		// f = ma, m = 1, f = a, v = int(a)
		xv += xf;
		yv += yf;
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


#define DRAW_FORCES
#define USE_INVSQRT

class AnimationParticlesMega2;
class ParticleSystem {
protected:
	float timeStep;
	vector<Particle> particles;
	vector< vector<Particle*> > bins;
	int width, height, k, xBins, yBins, binSize;
	float particleSize = 2.0f;
    
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
	void addRepulsionForce(const Particle& particle, float radius, float scale, bool doColor=false);
	void addRepulsionForce(float x, float y, float radius, float scale, bool doColor=false);
	void addAttractionForce(const Particle& particle, float radius, float scale, bool doColor=false);
	void addAttractionForce(float x, float y, float radius, float scale, bool doColor=false);
	void addForce(const Particle& particle, float radius, float scale, bool doColor=false);
	void addForce(float x, float y, float radius, float scale, bool doColor=false);
	void update();
	
	void setParticleSize(float s_){particleSize = s_;}
	float getParticleSize(){return particleSize;}
 
	void draw();

	ofColor m_color;
};

inline float InvSqrt(float x){
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}

class ParticleForce
{
	public:
		ParticleForce		(float anchorx, float anchory);

	    ofVec2f             m_anchor;
		float				m_volume, m_volumeTarget;
		ofColor				m_color;
	
		void				update					(float dt);
};


class AnimationParticlesMega2 : public Animation
{
public:
    
    AnimationParticlesMega2(string name);
	~AnimationParticlesMega2();
 
    virtual	void			VM_enter				();
    virtual void			VM_update				(float dt);
    virtual void			VM_draw					(float w, float h);
    virtual	void			VM_exit					();
    virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);
	virtual void			onVolumAccumEvent		(string deviceId);


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
	float					m_particlesSize;
		
	map<string, ParticleForce*>	m_mapParticleForce;
	ofVbo					m_particlesVbo;
	ofVec3f*				m_particlesPos;
	ofFloatColor*			m_particlesColor;
	
	float					m_colorRadiusFactor;
	
};
