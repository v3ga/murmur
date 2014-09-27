//
//  animationOrbit.h
//  murmur
//
//  Created by Julien on 25/09/2014.
//
//

#pragma once

#include "animations.h"

class ParticleOrbit;
class ParticlePath;

//--------------------------------------------------------------
class AnimationOrbit : public Animation
{
    public:
    
        AnimationOrbit(string name);
		~AnimationOrbit();
 
        virtual	void			VM_enter				();
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
        virtual	void			VM_exit					();
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);
		virtual	void			createUICustom			();

				void			drawOrbit				(ParticleOrbit* pOrbit);

		ParticlePath*			mp_testParticle;

		ParticleOrbit*			getOrbitForDevice		(Device*);
		ParticleOrbit*			getOrbitForDevice		(string deviceId);

		map<string, ParticleOrbit*>				m_orbits; // key is deviceId
		map<string, ParticleOrbit*>::iterator	m_orbitsIt;
};

//--------------------------------------------------------------
class ParticlePath
{
	public:
		ParticlePath			();


		void					setSegment				(int indexA, ParticleOrbit* pA, int indexB, ParticleOrbit* pB);
		void					update					(float dt);

		ofVec3f					m_pos;
		float					m_speed;

		ofVec2f					m_A;
		int						m_indexA;
		ParticleOrbit*			mp_orbitA;

		ofVec2f					m_B;
		int						m_indexB;
		ParticleOrbit*			mp_orbitB;

		ofVec2f					m_u;
		bool					m_isReachB;
};

//--------------------------------------------------------------
class ParticleOrbit
{
	public:
		virtual	void			computePoints			(){}
				vector<ofVec3f>&getPoints				(){return m_points;}
				void			setCenter				(ofVec3f center){m_center=center;computePoints();}
				ofVec3f&		getCenter				(){return m_center;}

	protected:
		ofVec3f					m_center;
		vector<ofVec3f>			m_points; // relative to local coordinates : origin is (0,0), not the Device point
};

//--------------------------------------------------------------
class ParticleOrbitEllipse : public ParticleOrbit
{
	public:
								ParticleOrbitEllipse	();

		virtual	void			computePoints			();
				void			setOffset				(ofVec3f offset);
				void			setRadius				(ofVec2f radius);
				void			setRotation				(float rot);

	private:
		ofVec3f					m_offset;
		ofVec2f					m_radius;
		float					m_rot;
};