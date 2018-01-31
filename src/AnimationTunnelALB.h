//
//  AnimationTunnelALB.h
//  murmur
//
//  Created by Julien on 03/02/2015.
//
//

#pragma once
#include "animations.h"
#include "volumeAccum.h"

class TunnelElementALB
{
    public:
        TunnelElementALB        (ofMesh* pMesh, ofVec3f pos, ofVec3f dir, float dirSpeed, int leftOrRight);
    
		void					setRotation	(ofMatrix4x4& m);
		void					update		(float dt);
		void					draw		();

		ofColor					m_color;
        ofVec3f                 m_pos;
        ofVec3f                 m_dir;
		float					m_dirSpeed;
		ofMesh*					mp_mesh;
		ofMatrix4x4				m_rot;
 
		int						m_leftOrRight;
};

class AnimationTunnelALB : public Animation
{

    public:
       AnimationTunnelALB(string name);
       ~AnimationTunnelALB();


        virtual	void			VM_enter				();
        virtual void			VM_update               (float dt);
        virtual void			VM_draw					(float w, float h);
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);
		virtual	void			onPropertyMidiModified	(classProperty* pProperty);
 
				void			handlePropertyModified	(string name);


        virtual void            createUICustom          ();
        virtual void            guiEvent                (ofxUIEventArgs &e);
    
    protected:
		vector<TunnelElementALB*> m_elements;
		void					deleteElements			();

		ofMesh*					mp_meshes[2];
		void					createMeshes			();
		void					deleteMeshes			();
		void					modifyMeshes			();
	
		ofVec3f					m_dirLeft,m_dirRight,m_posFar;
		float					m_dirAngle, m_dirSpeed;
	
        ofVec2f                 m_posAnchor;
    
        VolumeAccum             m_volAccum;
        static void             sM_volTriggerIn			(void*,VolumeAccum*);
 
        float                   m_zMax;
        float                   m_ampRotation;
        float                   m_speedWave;
    
        float                   m_volumeAccum;
        float                   m_volumeAccumTarget;

        float                   m_volAccumTh;
        float                   m_volAccumTrigger;
 
		float					m_h1Mesh,m_h2Mesh,m_wMesh;
		float					m_h2;
 
 		bool					m_emitLeft,m_emitRight;
 
		float					m_w, m_h;
 		ofMatrix4x4 			m_mRotationLeft,m_mRotationRight;
		void					updateRotations();
 
 
		ofCamera				m_cam;
 
		ofxUISlider*			mp_sliderW;
		ofxUISlider*			mp_sliderH2;
 
};
