//
//  animationTunnel.h
//  murmur
//
//  Created by Julien on 24/05/13.
//
//

#pragma once

#include "animations.h"
#include "MSAInterpolator.h"
#include "volumeAccum.h"

class TunnelElement
{
    public:
        TunnelElement           ();
    
    
        ofVec3f                 m_pos;
        float                   m_rot[16];
        float                   m_t;
};

class AnimationTunnel;
class Tunnel
{
	public:
	
		Tunnel					(AnimationTunnel*);
	
		virtual	void			VM_update				(float dt);
		virtual	void			VM_draw					();


		AnimationTunnel*		mp_animation;

		ofVec2f                 m_posAnchor;
        float                   m_volumeAccum;
        vector<TunnelElement*>  m_listTunnelElement;
};

class AnimationTunnel : public Animation
{
	friend class Tunnel;

    public:
        AnimationTunnel(string name);


        virtual	void			VM_enter				();
        virtual void			VM_update               (float dt);
        virtual void			VM_draw					(float w, float h);
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);

        virtual void            createUICustom          ();
        virtual void            guiEvent                (ofxUIEventArgs &e);
    
    protected:
		map<string, Tunnel*>	m_mapTunnels;
	
	
        ofVec2f                 m_posAnchor;
    
        VolumeAccum             m_volAccum;
        static void             sM_volTriggerIn(void*);
    
        AnimationSoundPlayer    m_soundPlayer;
    
        float                   m_zMax;
        float                   m_ampRotation;
        float                   m_speedWave;
    
        float                   m_volumeAccum;
        float                   m_volumeAccumTarget;

        float                   m_volAccumTh;
        float                   m_volAccumTrigger;
    
    
        msa::Interpolator3D     m_spline;
        float                   m_t;
        bool                    m_isCreateSpline;
    
        ofVec3f                 getTangentAt            (float t);
        void                    createSpline            (float w, float h);
        void                    computeBasisAt          (float* result, float t);

    
        vector<TunnelElement*>  m_listTunnelElement;

};

