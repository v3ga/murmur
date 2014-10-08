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
		void						setParentId		(string id){m_parentId=id;}
	
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
		string						m_parentId;
	
		float						m_cohesionGroup;
		float						m_separationGroup;
		float						m_alignmentGroup;

};


//--------------------------------------------------------------
class BoidGroup
{
	public:
		BoidGroup				();
	
		vector<Boid*>			m_boids;
		void					reset()
		{
			mp_particleOrbit = 0;
			m_boids.clear();
			m_boidState = boidState_rest;
			m_tBoidState = 0.0f;
			m_tBoidExcitationMax = 1.2f;
			m_fExcitation = 0.0f;
			m_fExcitationTarget = 0.0f;
		}

		void					setParticleOrbit(ParticleOrbit* p){mp_particleOrbit = p;}
		ParticleOrbit*			getParticleOrbit(){return mp_particleOrbit;}
		void 					add(Boid* pBoid){m_boids.push_back(pBoid);}
		void					update(float dt);
		void					excite	();
		void					calm	();
	
		enum
		{
			boidState_rest				= 0,
			boidState_excitation		= 1
		};
	
		ParticleOrbit*			mp_particleOrbit;

		int						m_boidState;
		float					m_tBoidState;
		float					m_tBoidExcitationMax;
	
		float					m_fExcitation;
		float					m_fExcitationTarget;
		float					m_fExcitationRise;
	
		ofVec2f					m_radiusOrbitOriginal;
	
		float					m_cohesionGroup;
		float					m_separationGroup;
		float					m_alignmentGroup;
	
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
		virtual void			onVolumAccumEvent		(string deviceId);
		virtual	void			createUICustom			();
				void			guiEvent				(ofxUIEventArgs &e);
				void			updateUI				();
				void            saveProperties          (string id);
				void            loadProperties          (string id);
				void			loadPropertiesOrbit		(string deviceId);
				void			groupBoidsByDevice		();
	
				void			drawOrbit				(ParticleOrbit* pOrbit);
	
				void			createParticlePathsForOrbit(ParticleOrbit* pOrbit);
				void			assignBoidsToPaths		();
				void			deleteParticlePaths		();

		vector<ParticlePath*>	m_particlePaths;
	

		ParticleOrbit*			getOrbitForDevice		(Device*);
		ParticleOrbit*			getOrbitForDevice		(string deviceId);

		map<string, ParticleOrbit*>				m_orbits; // key is deviceId
		map<string, ParticleOrbit*>::iterator	m_orbitsIt;
		map<string, float>						m_energies; // key is deviceId
		map<string, float>						m_deviceVolume,m_deviceVolumeTarget; // key is deviceId
		map<string, BoidGroup*>					m_boidsForDevice;
	
		BoidGroup*				getBoidsForDevice		(string deviceI);
	
	
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
		float					m_boidsDrawLineWidth;
		float					m_boidsSpeedFactor; // TEMP, testing
		int						m_boidsNbParticlesPath;
		float					m_boidsSpeedMin;
		float					m_boidsSpeedMax;
	
		bool					m_reverseForms;
		float					m_rotationForms, m_widthForms, m_heightForms;
	
		float					m_deviceVolumeSpeed;
	

	protected:
		Device*					mp_deviceCurrent;
		ofxUILabel*				mp_labelDeviceId;
		ofxUIToggle*			mp_toggleFormReverse;
		ofxUISlider*			mp_sliderFormRot;
		ofxUISlider*			mp_sliderFormWidth;
		ofxUISlider*			mp_sliderFormHeight;
	
		ofxUIMovingGraph*		mp_mgEnergy;
	
		ofxXmlSettings			m_extraData;
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
		string					getOrbitId				();
	
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
				string			getId					(){return m_id;}
				void			setId					(string id){m_id=id;}
				vector<ofVec3f>&getPoints				(){return m_points;}
				void			setCenter				(ofVec3f center){m_center=center;computePoints();}
				ofVec3f&		getCenter				(){return m_center;}
				void			setReverse				(bool is);
				bool			getReverse				(){return m_isReverse;}
				virtual void	setScale				(float s){}
				virtual	void	save					(ofxXmlSettings& settings){}
				virtual	void	load					(ofxXmlSettings& settings){}
				virtual void	setRadius				(ofVec2f radius){}
				virtual	ofVec2f getRadius				(){return ofVec2f(0.0f,0.0f);}

	protected:
		string					m_id;
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
				void			setRotation				(float rot);
				float			getRotation				(){return m_rot;}
				void			setScale				(float s);
				void			setRadius				(ofVec2f radius);
				ofVec2f 		getRadius				(){return m_radius;}

				void			save					(ofxXmlSettings& settings);
				void			load					(ofxXmlSettings& settings);

	private:
		ofVec3f					m_offset;
		ofVec2f					m_radius;
		float					m_rot;
		float					m_scale;
};