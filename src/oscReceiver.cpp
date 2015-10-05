//
//  oscReceiver.cpp
//  murmur
//
//  Created by Julien on 14/04/13.
//
//

#include "oscReceiver.h"
#include "globals.h"
#include "surface.h"
#include "deviceEcho.h"
#include "testApp.h"


//--------------------------------------------------------------
Device* oscReceiver::discoverDevice(string id, int nbLeds)
{
    DeviceManager* pDeviceManager = Globals::instance()->mp_deviceManager;
   if (pDeviceManager == 0)
   	return 0;

   Device* pDevice = pDeviceManager->getDeviceById(id);
   if (pDevice == 0)
   {
	   OFAPPLOG->begin("oscReceiver::discoverDevice("+id+","+ofToString(nbLeds)+")");
	
	   // TODO : type ? -> factory
	   if (Globals::instance()->mp_app->isSimulation == false)
	   {
		   OFAPPLOG->println("> New Device found ! id='"+id+"'");
		   
		   DeviceEcho* pDeviceEcho = new DeviceEcho(id, nbLeds, 4.0f/float(nbLeds-1));
		   pDeviceEcho->setupSenderOSC(m_oscMessage.getRemoteIp(), 1235);
		   pDeviceEcho->loadXML("Config/devices/");

		   // Add device to list
		   pDeviceManager->addDevice(pDeviceEcho);
		   
		   // TEMP / TODO : attach to surface main
		   if (Globals::instance()->mp_app->mp_surfaceMain)
			   Globals::instance()->mp_app->mp_surfaceMain->addDevice(pDeviceEcho);

		   // Create a node in visualisation scene
		   Globals::instance()->mp_app->createDeviceNodeInScene(pDeviceEcho);
	   }
	   OFAPPLOG->end();
   }
   return pDevice;
}

//--------------------------------------------------------------
void oscReceiver::update()
{
    DeviceManager* pDeviceManager = Globals::instance()->mp_deviceManager;
	
    if (hasWaitingMessages())
    {
		while(getNextMessage(&m_oscMessage))
		{
            int indexArg = 0;
			
			LOG_MESSAGE_OSC(m_oscMessage,true);

            if (m_oscMessage.getAddress() == OSC_ADDRESS_SET_DEVICE_PROP)
            {
                Device* pDevice = pDeviceManager->getDeviceById( m_oscMessage.getArgAsString(indexArg++) );
                if (pDevice)
				{
                    // Property
                    string propName = m_oscMessage.getArgAsString(indexArg++);

					if (propName == "mute")
                    {
                        pDevice->setSoundInputMuteOSC( m_oscMessage.getArgAsInt32(indexArg)==1 ? true : false );
                    }
					else
					if (propName == "useRawVol")
                    {
                        pDevice->setSoundInputUseRawVolumeOSC( m_oscMessage.getArgAsInt32(indexArg)==1 ? true : false );
                    }
					else
					if (propName == "volMax")
                    {
                        pDevice->setSoundInputVolumeMaxOSC( m_oscMessage.getArgAsFloat(indexArg) );
                    }
					else
					if (propName == "volMaxMax")
                    {
                        pDevice->setSoundInputVolumeMaxMaxOSC( m_oscMessage.getArgAsFloat(indexArg) );
                    }
                    else
                    if (propName == "volHistorySize")
                    {
                        pDevice->setSoundInputVolHistorySizeOSC( m_oscMessage.getArgAsInt32(indexArg) );
                    }
                    else
                    if (propName == "volHistoryTh")
                    {
                        pDevice->setSoundInputVolHistoryThOSC( m_oscMessage.getArgAsFloat(indexArg) );
                    }
                    else
                    if (propName == "volHistoryPingTh")
                    {
                        pDevice->setVolHistoryPingThOSC( m_oscMessage.getArgAsFloat(indexArg) );
                    }
                    else
                    if (propName == "enableStandbyMode")
                    {
                        pDevice->setEnableStandbyModeOSC( m_oscMessage.getArgAsInt32(indexArg)==1 ? true : false );
                    }
                    else
                    if (propName == "timeStandby")
                    {
                        pDevice->setTimeStandbyOSC( m_oscMessage.getArgAsFloat(indexArg) );
                    }
                    else
                    if (propName == "sampleVolStandby")
					{
                        pDevice->setSampleVolumeStandbyOSC( m_oscMessage.getArgAsFloat(indexArg) );
					}
                    else
                    if (propName == "volStandup")
					{
                        pDevice->setStandupVolOSC( m_oscMessage.getArgAsFloat(indexArg) );
					}
                    else
                    if (propName == "enableColor")
					{
						int value = m_oscMessage.getArgAsInt32(indexArg++);
						pDevice->enableColorOSC(value>0 ? true : false);
					}
                    else
                    if (propName == "color")
					{
						float hue = m_oscMessage.getArgAsFloat(indexArg++);
						float sat = m_oscMessage.getArgAsFloat(indexArg);
                        pDevice->setColorHueSaturationOSC(hue,sat);

						// ofLog() << "(hue,sat)=" << hue << ',' << sat;
					}
					else
					if (propName == "invertPacketsVolume")
					{
						int value = m_oscMessage.getArgAsInt32(indexArg++);
						pDevice->invertPacketsVolumeOSC(value>0 ? true : false);
					}
					else
					if (propName == "reversePacketsDir")
					{
						int value = m_oscMessage.getArgAsInt32(indexArg++);
						pDevice->reversePacketsDirOSC(value>0 ? true : false);
					}
                    else
                    if (propName == "enableGenerative")
					{
						int value = m_oscMessage.getArgAsInt32(indexArg++);
						pDevice->setGenerativeOSC(value>0 ? true : false);
					}
                    else
                    if (propName == "bpmEnable")
					{
						int value = m_oscMessage.getArgAsInt32(indexArg++);
						pDevice->setBPMEnableOSC(value>0 ? true : false);
					}
                    else
                    if (propName == "bpm")
					{
						int value = m_oscMessage.getArgAsInt32(indexArg++);
						pDevice->setBPMOSC(value);
					}

                }
            }
            else
            if (m_oscMessage.getAddress() == OSC_ADDRESS_SEND_PACKETS_BEGIN)
            {
				string deviceId = m_oscMessage.getArgAsString(indexArg++);
				int deviceNbLeds = m_oscMessage.getArgAsInt32(indexArg++);

                Device* pDevice = discoverDevice(deviceId, deviceNbLeds);
				if (pDevice)
					pDevice->onReceivePacketBegin();
			}
            else
            if (m_oscMessage.getAddress() == OSC_ADDRESS_SEND_PACKETS_END)
            {
				string deviceId = m_oscMessage.getArgAsString(indexArg++);
                Device* pDevice = pDeviceManager->getDeviceById( deviceId );
				if (pDevice)
				{
					pDevice->onReceivePacketEnd();

					// Get the surface of device and update animation
					Surface* pSurface = Globals::instance()->mp_app->getSurfaceForDevice(pDevice);
				
					if (pSurface && pDevice->isGenerative())
					{
						pSurface->onNewPacket( pDevice->getLastPacket(), pDevice->m_id );
					}
				}
			}
            else
            if (m_oscMessage.getAddress() == OSC_ADDRESS_SEND_PACKETS)
            {
                if (pDeviceManager)
                {
                    string deviceId = m_oscMessage.getArgAsString(indexArg++);
                    Device* pDevice = pDeviceManager->getDeviceById( deviceId );
				 
                    // Decode new message
                    if (pDevice)
                    {
                        int nbPackets = (m_oscMessage.getNumArgs() - 1)/4; // TODO : Be careful with this
                        for (int i=0;i<nbPackets;i++)
                        {
                            m_packetTemp.m_volume = m_oscMessage.getArgAsFloat(indexArg++);
							//m_packetTemp.m_color.setHue			( m_oscMessage.getArgAsFloat(indexArg++) );
							//m_packetTemp.m_color.setSaturation	( m_oscMessage.getArgAsFloat(indexArg++) );
							//m_packetTemp.m_color.setBrightness	( m_oscMessage.getArgAsFloat(indexArg++) );

							m_packetTemp.m_color.set(m_oscMessage.getArgAsFloat(indexArg++),m_oscMessage.getArgAsFloat(indexArg++),m_oscMessage.getArgAsFloat(indexArg++));
						 
						 	// ofLog() << m_packetTemp.m_color;
						 
                            // Send to device
                            pDevice->onReceivePacket(&m_packetTemp);
                        }
                    }
                }
            }
			else
			if (m_oscMessage.getAddress() == OSC_ADDRESS_RESET_PING)
			{
                if (pDeviceManager)
                {
                    string deviceId = m_oscMessage.getArgAsString(indexArg++);
                    Device* pDevice = pDeviceManager->getDeviceById( deviceId );
					if (pDevice)
						pDevice->resetVolHistoryPingOSC();
				}
			}


        }
    }
}
