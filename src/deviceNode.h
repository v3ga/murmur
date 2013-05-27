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
    
    protected:
        Device*                     mp_device;
        msa::physics::World3D		physics;
};
