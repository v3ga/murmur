//
//  animationGravitatory.h
//  murmur
//
//  Created by Julien on 23/06/2015.
//
//

#pragma once
#include "animations.h"

class AnimationGravitatory;
class ParticleGravitatory
{
	public:
		ParticleGravitatory	(AnimationGravitatory* pParent, string deviceId, float x, float y, ofColor c);

		void			update		(float dt);
		void			draw		();


		AnimationGravitatory*	mp_parent;

		string			m_deviceId;
		ofVec2f			m_pos;
		float			m_angle,m_angleSpeed;
		float			m_age;
		float			m_size;
		float			m_ageMax;
		ofColor			m_color;

};

class AnimationGravitatory : public Animation
{
	public:
       AnimationGravitatory			(string name);
       ~AnimationGravitatory		();


		void            createUICustom          ();
		void			VM_update				(float dt);
		void			VM_draw					(float w, float h);
        void            guiEvent                (ofxUIEventArgs &e);

		void            onNewPacket				(DevicePacket*, string deviceId, float xNorm, float yNorm);

		map<string, vector<ParticleGravitatory*> > m_particlesDevice;

		float			m_sizeFactor;
		float			m_alphaFactor;
		float			m_angleSpeedFactor;
};