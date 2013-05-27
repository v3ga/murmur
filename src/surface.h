/*
 *  surface.h
 *  murmur
 *
 *  Created by Julien on 10/04/13.
 *  Copyright 2013 2Roqs. All rights reserved.
 *
 */

#pragma once
#include "ofFbo.h"
#include "animations.h"
#include "device.h"

class Surface
{
	public:
		Surface		(string id, int wPixels, int hPixels);

    
        ofFbo&          getOffscreen    (){return m_fbo;}
        void            setDimensions	(int w, int h);

        void            setup           ();
        void            update          (float dt);
        void            renderOffscreen ();
        void            addDevice       (Device*);
        vector<Device*>&getListDevices  (){return m_listDevices;}
        void            onNewPacket     (DevicePacket*, string deviceId);
        void            drawDevicePointSurface();
        void            drawCacheLEDs   (float d);
        float           getWidthPixels  (){return m_fbo.getWidth();}
        float           getHeightPixels (){return m_fbo.getHeight();}
        string          getId           (){return m_id;}
    
        void            setDurationTransition(float d){m_durationTransition=d;}
        void            setDurationAnimation(float d){m_durationAnimation=d;}
        void            setTimelineActive(bool is);
        bool            isTimelineActive(){return m_isTimelineActive;}
    
        AnimationManager& getAnimationManager(){return m_animationManager;}
    
        void            saveAnimationsProperties();
        void            loadAnimationsProperties();
	
    private:
        string              m_id;
		ofFbo               m_fbo;
        AnimationManager    m_animationManager;
        vector<Device*>     m_listDevices;
        ofVec2f             m_devicePoint;
    
        float               m_durationTransition;
        float               m_durationAnimation;
        bool                m_isTimelineActive;
        bool                m_isAnimationTransiting;
        float               m_timeAnimation;
    
};