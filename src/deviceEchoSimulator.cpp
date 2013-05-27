//
//  deviceEchoSimulator.cpp
//  murmur
//
//  Created by Julien on 15/04/13.
//
//

#include "deviceEchoSimulator.h"


//--------------------------------------------------------------
DeviceEchoSimulator::DeviceEchoSimulator(string id, int nbLEDs, float distLEDs) :
Device(id,nbLEDs,distLEDs)
{
    m_oscSender.setup("127.0.0.1", 1234);
}


//--------------------------------------------------------------
void DeviceEchoSimulator::onReceivePacket(DevicePacket*)
{
    // DO NOTHING
}

