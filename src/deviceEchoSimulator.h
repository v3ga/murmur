//
//  deviceEchoSimulator.h
//  murmur
//
//  Created by Julien on 15/04/13.
//
//

#pragma once
#include "deviceEcho.h"

class DeviceEchoSimulator : public Device
{
    public:
        DeviceEchoSimulator              (string id, int nbLEDs, float distLEDs);

        void                            onReceivePacket(DevicePacket*);

};