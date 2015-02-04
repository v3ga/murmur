//
//  oscReceiver.h
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#pragma once

#include "ofxOsc.h"
#include "oscDefs.h"
#include "device.h"

class oscReceiver  : public ofxOscReceiver
{
    public:
        void                update      		();
		Device*				discoverDevice		(string id, int nbLeds);
    
private:
        ofxOscMessage		m_oscMessage;
        DevicePacket        m_packetTemp;
};

