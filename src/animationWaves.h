//
//  animationWaves.h
//  murmur
//
//  Created by Julien on 14/07/2015.
//
//

#pragma once
#include "animations.h"
#include "volumeAccum.h"

class AnimationWaves;

class WaveManager;
class Wave
{
	public:
		Wave					(WaveManager* pParent, float x, float y);

		void					setColor		(ofColor c){m_color = c;}
		void					setPhase		(float p){m_phase = p;}
		void					setDir			(float dx,float dy){m_dir.set(dx,dy);}
		void					setAgeMax		(float a){m_ageMax=a;}
		bool					isOld			(){return m_age>=m_ageMax;}
		void					setSpeed		(float rs){m_radiusSpeed = rs;}
		void					setNbPoints		(int nb);

		void					updatePoints	();
		void					update			(float dt);

 
 		WaveManager*			mp_parent;
 		int						m_nbPoints;
 
		ofVec3f					m_loc;
		vector<ofVec3f>			m_points;
 
		float					m_radius;
		float					m_radiusSpeed;
		ofVec3f					m_dir;
		float					m_phase;
 
 		ofColor					m_color;
 
		float					m_age, m_ageMax;
};

class WaveManager
{
	public:
		WaveManager				(AnimationWaves* pAnimParent);
		~WaveManager			();

		void					update				(float dt);
		void					draw				();
		void            		onNewPacket			(DevicePacket*, ofColor deviceColor, float xNorm, float yNorm);

		static void				sM_volumeTriggerIn	(void*, VolumeAccum*);
		void					computeMesh			();
		void					setNbPoints			(int nb);
 
 		AnimationWaves*			mp_parent;
		VolumeAccum				m_volumeAccum;
 
 		vector<Wave*>			m_waves;
		ofVboMesh				m_mesh;
 
		ofVec2f					m_lastPacketPos;
		ofColor					m_lastPacketColor;
};

class AnimationWaves : public Animation
{
	public:
       AnimationWaves			(string name);
       ~AnimationWaves			();


		void            createUICustom          ();
		void			VM_update				(float dt);
		void			VM_draw					(float w, float h);
        void            guiEvent                (ofxUIEventArgs &e);

		void            onNewPacket				(DevicePacket*, string deviceId, float xNorm, float yNorm);

		map<string, WaveManager* > 				m_wavesDevice;
		map<string, WaveManager* >::iterator	m_wavesDeviceIt;
 
 
		float			m_waveDirAmp;
		float			m_speed;
		float			m_ageMax;
		float			m_valueTriggerIn;
		float			m_valueTriggerOut;
		bool			m_enableTimeResetAccum;
		float			m_timeResetAccum;
		float			m_nbWavePoints;
		float			m_lineWidthWave;
};