//
//  deviceEcho.cpp
//  murmur
//
//  Created by Julien on 15/04/13.
//
//

#include "deviceEcho.h"

//--------------------------------------------------------------
DeviceEcho::DeviceEcho(string id, int nbLEDs, float distLEDs) : Device(id,nbLEDs,distLEDs)
{
    m_connectSPI = false;
    m_isDebugSinewave = false;
    m_isDebugSinewaveAngle = 0.0f;
}


//--------------------------------------------------------------
void DeviceEcho::loadXMLData(ofxXmlSettings& settings)
{
/*	loadXMLSoundInput(settings);
	loadXMLColor(settings);
*/
	Device::loadXMLData(settings);
}

//--------------------------------------------------------------
void DeviceEcho::setupSenderOSC(string ip, int port)
{
    m_oscSender.setup(ip, port);
}

//--------------------------------------------------------------
void DeviceEcho::setupLEDs()
{
    bool connectSPI = false;
	if(	m_spi.connect())
	{
		ofLogNotice()<<"connected to SPI";
		m_connectSPI = true;
	}
 
    if (m_connectSPI)
    {
    
        for (int i=0;i<m_nbLEDs*3;i++)
        {
            m_spiData.push_back(0);
        }
    
        // +1 for latch
        m_spiData.push_back(0);

    
        m_spiColors.assign(m_nbLEDs,ofColor());
    }


}

//--------------------------------------------------------------
void DeviceEcho::update(float dt)
{
    // Parent
    //Device::update(dt);
    checkForActivity(dt);


    bool isSample = false;
    if (m_isDebugSinewave)
    {
        if (mp_soundInput)
        {
            int nbVolHistory = mp_soundInput->getVolHistory().size();
            float value=0.0f;
            float phase = 0.0f;
            for (int i=0;i<nbVolHistory;i++)
            {
                value = 0.5f*(1.0f+sin( ofDegToRad(m_isDebugSinewaveAngle-phase) ));
                mp_soundInput->setVolHistoryValue(i, value);
                phase+= 360.0f / float(nbVolHistory-1);
            }
            
            m_isDebugSinewaveAngle += 0.5f;
            isSample = true;
        }
    }
    else
    {
        if (mp_soundInput)
        {
            mp_soundInput->update();
            isSample = true;
        }
    }

    if(isSample)
    {
        // Sample them and writes into packet
        sampleSoundInput();
        
        // Send packets / values to network
        sendPacketsOSC();
    }
}

//--------------------------------------------------------------
void DeviceEcho::updateLEDs()
{
    // LEDs
    int nbPackets = m_listPackets.size();
    if (m_connectSPI && nbPackets == m_nbLEDs) // Sanity check
    {
        // Tranforms packets -> colors
        DevicePacket* pDevicePacket=0;
        for (int i=0; i<nbPackets; i++)
        {
            pDevicePacket = m_listPackets[i];
            
/*
            m_spiColors[i][0] = pDevicePacket->m_volume*255.0f;
            m_spiColors[i][1] = pDevicePacket->m_volume*255.0f;
            m_spiColors[i][2] = pDevicePacket->m_volume*255.0f;
*/
            m_spiColors[i][0] = pDevicePacket->m_color[0];
            m_spiColors[i][1] = pDevicePacket->m_color[1];
            m_spiColors[i][2] = pDevicePacket->m_color[2];
        }
        
        // Tranforms colors -> uint_8 buffer
        int offsetData = 0;
        for (int i=0;i<m_spiColors.size();i++)
        {
            m_spiData[offsetData] = m_spi.toGamma( (u_int8_t) m_spiColors[i][1] );
            m_spiData[offsetData+1] = m_spi.toGamma( (u_int8_t) m_spiColors[i][0] );
            m_spiData[offsetData+2] = m_spi.toGamma( (u_int8_t) m_spiColors[i][2] );
            offsetData+=3;
        }
        
        // Send data
        m_spi.send(m_spiData);
    }
}


