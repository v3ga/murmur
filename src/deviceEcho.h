//
//  deviceEcho.h
//  murmur
//
//  Created by Julien on 15/04/13.
//
//

#pragma once
#include "device.h"
#include "soundInput.h"
#include "ofxLPD8806.h"


class DeviceEcho : public Device
{
    public:
        DeviceEcho  (string id, int nbLEDs, float distLEDs);

    
        void            setDebugSinewave(bool is){m_isDebugSinewave = is;}
        void            setupSenderOSC(string ip, int port);
        void            setupLEDs();
        void            updateLEDs();
    
        void            update(float dt);
    
    protected:
    
        // LEDs data
        ofxLPD8806      m_spi;
        vector<ofColor> m_spiColors;
        vector<uint8_t> m_spiData;
        bool            m_connectSPI;
    
        // Sinewave
        bool            m_isDebugSinewave;
        float           m_isDebugSinewaveAngle;
};