//
//  deviceNode.h
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#pragma once
#include "ofMain.h"
#include "device.h"
#include "MSAPhysics3D.h"


class DeviceNode : public ofNode
{
    public:
        DeviceNode                  (Device*);
    
        void                        createChain                 (ofVec3f ptAttach, ofVec3f dir);
        void                        setPositionNodeSoundInput   (ofVec3f newPosition);
        void                        setPositionNodeSurface      (ofVec3f newPosition);
        void                        customDraw                  ();
        Device*                     getDevice                   (){return mp_device;}
		string						getDeviceId					(){if (mp_device) return mp_device->m_id; return "???";}

		void						setDrawLights				(bool is=true);
 
    protected:
        Device*                     mp_device;
        msa::physics::World3D		physics;
 

		int							m_drawFlags;
		enum
		{
			E_drawLights			= 0x01
		};
};
