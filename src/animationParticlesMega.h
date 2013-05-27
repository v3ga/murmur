//
//  animationParticlesMega.h
//  murmur
//
//  Created by Julien on 17/05/13.
//
//
#pragma once
#include "animations.h"
#include "MSAOpenCL.h"


typedef struct{
	float2 vel;
	float mass;
	float dummy;		// need this to make sure the float2 vel is aligned to a 16 byte boundary
} Particle;


#define NUM_PARTICLES (1000*1000)


class AnimationParticlesMega : public Animation
{
public:
    
    AnimationParticlesMega(string name);
    
    virtual	void			VM_enter				();
    virtual void			VM_update				(float dt);
    virtual void			VM_draw					(float w, float h);
    virtual	void			VM_exit					();
    virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);
    
    
    void                    createParticles         ();
    

private:
    ofVec2f                 m_anchor;
    ofVec2f                 m_resolution;
    

    bool                    m_isCLInit;
    
    float2                  mousePos;
    float2                  dimensions;
    
    msa::OpenCL             opencl;
    msa::OpenCLKernel       *kernelUpdate;
    
    
    Particle                particles[NUM_PARTICLES];
    msa::OpenCLBuffer       clMemParticles;		// stores above data
    
    
    float2                  particlesPos[NUM_PARTICLES];
    msa::OpenCLBuffer       clMemPosVBO;		// stores above data
    
    GLuint                  vbo[1];

};