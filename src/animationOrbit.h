//
//  animationOrbit.h
//  murmur
//
//  Created by Julien on 25/09/2014.
//
//

#pragma once

#include "animations.h"
#include "Boid.h"

class ParticleOrbit;
class ParticlePath;
class AnimationOrbit;

//--------------------------------------------------------------
class BoidOrbit : public Boid
{
	public:
		BoidOrbit					(AnimationOrbit*, float x, float y);

		void						setSpeedMinMax	(float speedMin, float speedMax);
		void						setForceMax		(float forceMax);
	
		void						follow			(ofVec2f& target, vector<Boid*>& boids);
		void 						flock			(vector<Boid*>& boids);
		void						draw			();

		AnimationOrbit*				mp_animation;
		vector<ofVec2f>				m_locations;
		int							m_locationsNbMax;
		ofPolyline					m_polyline;

		ParticlePath*				mp_particlePath;
};

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
				void			guiEvent				(ofxUIEventArgs &e);
				void			updateUI				();
	
				void			drawOrbit				(ParticleOrbit* pOrbit);

		ParticlePath*			mp_testParticle;
		vector<ParticlePath*>	m_particlePaths;
	

		ParticleOrbit*			getOrbitForDevice		(Device*);
		ParticleOrbit*			getOrbitForDevice		(string deviceId);

		map<string, ParticleOrbit*>				m_orbits; // key is deviceId
		map<string, ParticleOrbit*>::iterator	m_orbitsIt;
	
		bool					m_isDrawDebug;
		bool					m_isFrameBlending;
		float					m_frameBlending;

		vector<Boid*>			m_boids;
		float					m_boidsSeparation;
		float					m_boidsCohesion;
		float					m_boidsAlignement;
		float					m_boidsMaxSpeedMin;
		float					m_boidsMaxSpeedMax;
		float					m_boidsForceMax;
	
		float					m_rotationForms, m_widthForms, m_heightForms;


	protected:
		Device*					mp_deviceCurrent;
		ofxUILabel*				mp_labelDeviceId;
		ofxUISlider*			mp_sliderFormRot;
		ofxUISlider*			mp_sliderFormWidth;
		ofxUISlider*			mp_sliderFormHeight;
};

//--------------------------------------------------------------
class ParticlePath
{
	public:
		ParticlePath			();


		void					setSegment				(int indexA, ParticleOrbit* pA, int indexB, ParticleOrbit* pB);
		void					update					(float dt);

		ofVec2f					m_pos;
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
				void			setWidth				(float w){m_radius.x = w; computePoints();}
				void			setHeight				(float h){m_radius.y = h; computePoints();}
				void			setRadius				(ofVec2f radius);
				ofVec2f			getRadius				(){return m_radius;}
				void			setRotation				(float rot);
				float			getRotation				(){return m_rot;}

	private:
		ofVec3f					m_offset;
		ofVec2f					m_radius;
		float					m_rot;
};