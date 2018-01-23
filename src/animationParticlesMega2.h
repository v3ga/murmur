//
//  animationParticlesMega2.h
//  murmur
//
//  Created by Julien on 17/05/13.
//
//

#pragma once
#define DRAW_FORCES
#define USE_INVSQRT

#include "animations.h"
#include "ParticleSystem.h"


class AnimationParticlesMega2;

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
	virtual void			updateUI				();
    virtual void            guiEvent                (ofxUIEventArgs &e);

    
    void                    createParticles         ();
    
    
private:
	ofxUISlider*			mp_sliderAmpAttraction;
	ofxUISlider*			mp_sliderAmpRepulsion;
	ofxUISlider*			mp_sliderRadiusRepulsion;
	ofxUISlider*			mp_sliderParticleSize;

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
	
	VolumeAccum*			mp_volumeAccum;
	
	float					m_colorRadiusFactor;
	
};
