//
//  animationAgentsFF.h
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#pragma once
#include "animations.h"
#include "ParticleSystem.h"
#include "ofxVectorField.h"

class AnimationAgentsFF : public Animation
{
	public:
		AnimationAgentsFF			(string name);
		~AnimationAgentsFF			();
 
 
 		virtual	void			createUICustom			();
 
 		virtual	void			VM_enter				();
 		virtual	void			VM_exit					();
		virtual void			VM_update				(float dt);
        virtual void			VM_drawBefore			(float w, float h);
        virtual void			VM_draw					(float w, float h);
        virtual void            guiEvent                (ofxUIEventArgs &e);
 
	protected:
		float					m_w, m_h;
		ofFbo					m_fboParticles;
		float					m_blending;

		ofxVectorField			m_vectorField;
		float					m_vectorFieldAnim;
		float					m_vectorFieldAnim2;
		bool					m_bDrawVectorField;
		ofImage					m_vectorFieldImage;

	    ParticleSystem	 		m_particleSystem;
		ofVbo					m_particlesVbo;
		ofVec3f*				m_particlesPos;
		bool					m_bParticlesInit;
		int						m_kParticles;
		float					m_particlesSpeedFactor;
		float					m_particleSpeedMax;
		float					m_particlesSize;
 
		bool					m_bResetParticles;
 
 		ofxUILabelButton*		mp_btnReset;
 		vector<string> 			m_resetModes;
		string					m_resetMode;
 
		bool					isToggleResetMode		(string name);

 
 
				void			resetParticles			(string name);
				void			createParticles			();
};
