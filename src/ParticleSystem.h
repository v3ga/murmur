//
//  ParticleSystem.h
//  murmur
//
//  Created by Julien on 02/10/2015.
//
//

#pragma once
#include "ofMain.h"
#include "Particle.h"
#include "ParticleForce.h"


class ParticleSystem
{
   protected:
	   float timeStep;
	   vector<Particle> particles;
	   vector< vector<Particle*> > bins;
	   int width, height, k, xBins, yBins, binSize;
	   float particleSize = 4.0f;
	   
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

inline float InvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}
