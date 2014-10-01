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
		void						setPath			(ParticlePath* p){mp_particlePath=p;}
		void						setLocationsNbMax(int n);
	
		void						follow			(ofVec2f& target, vector<Boid*>& boids);
		void						follow			(vector<Boid*>& boids);
		void 						flock			(vector<Boid*>& boids);
		void						update			(float dt);
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
				void            saveProperties          (string id);
				void            loadProperties          (string id);
	
				void			drawOrbit				(ParticleOrbit* pOrbit);

		ParticlePath*			mp_testParticle;
		vector<ParticlePath*>	m_particlePaths;
	

		ParticleOrbit*			getOrbitForDevice		(Device*);
		ParticleOrbit*			getOrbitForDevice		(string deviceId);

		map<string, ParticleOrbit*>				m_orbits; // key is deviceId
		map<string, ParticleOrbit*>::iterator	m_orbitsIt;
	
		map<string, float>						m_energies; // key is deviceId
	
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
		float					m_boidsDrawAlpha;
		float					m_boidsSpeedFactor; // TEMP, testing
	
		float					m_rotationForms, m_widthForms, m_heightForms;
	
	


	protected:
		Device*					mp_deviceCurrent;
		ofxUILabel*				mp_labelDeviceId;
		ofxUISlider*			mp_sliderFormRot;
		ofxUISlider*			mp_sliderFormWidth;
		ofxUISlider*			mp_sliderFormHeight;
	
		ofxUIMovingGraph*		mp_mgEnergy;
};

//--------------------------------------------------------------
class ParticlePath
{
	public:
		ParticlePath			();


		void					setSegment				(ParticleOrbit* pA, int indexA, ParticleOrbit* pB, int indexB);
		void					setSegment				(ParticleOrbit* pA, int indexA);
		void					setSpeed				(float s){m_speed=s;}
		void					update					(float dt);
		int						getNbTurns				(){return m_nbTurns;}
	
		int						m_nbTurns;

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
				void			setReverse				(bool is){m_isReverse=is;}
				bool			getReverse				(){return m_isReverse;}
				virtual	void	save					(ofxXmlSettings& settings){}
				virtual	void	load					(ofxXmlSettings& settings){}

	protected:
		ofVec3f					m_center;
		vector<ofVec3f>			m_points; // relative to local coordinates : origin is (0,0), not the Device point
		bool					m_isReverse;

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

				void			save					(ofxXmlSettings& settings);
				void			load					(ofxXmlSettings& settings);

	private:
		ofVec3f					m_offset;
		ofVec2f					m_radius;
		float					m_rot;
};