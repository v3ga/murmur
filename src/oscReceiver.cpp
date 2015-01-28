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
                 
					if (propName == "volMax")
                    {
                        pDevice->setSoundInputVolumeMaxOSC( m_oscMessage.getArgAsFloat(indexArg) );
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
                    if (propName == "enableStandbyMode")
                    {
                        pDevice->setEnableStandbyModeOSC( m_oscMessage.getArgAsInt32(indexArg)==1 ? true : false );
                    }
                    else
                    if (propName == "timeStandby")
                    {
                        //printf("timeStandby = %.3f - ", m_oscMessage.getArgAsFloat(indexArg));
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
                }
            }
            else
            if (m_oscMessage.getAddress() == OSC_ADDRESS_SEND_PACKETS_BEGIN)
            {
				string deviceId = m_oscMessage.getArgAsString(indexArg++);
                Device* pDevice = pDeviceManager->getDeviceById( deviceId );
				if (pDevice)
				{
					pDevice->onReceivePacketBegin();
				}
			}
            else
            if (m_oscMessage.getAddress() == OSC_ADDRESS_SEND_PACKETS_END)
            {
				string deviceId = m_oscMessage.getArgAsString(indexArg++);
                Device* pDevice = pDeviceManager->getDeviceById( deviceId );
				if (pDevice)
				{
					pDevice->onReceivePacketEnd();
				}
			}
            else
            if (m_oscMessage.getAddress() == OSC_ADDRESS_SEND_PACKETS)
            {
                if (pDeviceManager)
                {
                    string deviceId = m_oscMessage.getArgAsString(indexArg++);
                    Device* pDevice = pDeviceManager->getDeviceById( deviceId );

                    // Not found, create the device now
                    if (pDevice == 0)
                    {
                        // TODO : type ? -> factory
                        if (Globals::instance()->mp_app->isSimulation == false)
                        {
                            int nbLEDs = (m_oscMessage.getNumArgs() - indexArg)/4; // WARNING with this
							OFAPPLOG->println("> New Device found ! id='"+deviceId+"'");
                            
                            DeviceEcho* pDeviceEcho = new DeviceEcho(deviceId, nbLEDs, 4.0f/float(nbLEDs-1));
                            pDeviceEcho->setupSenderOSC(m_oscMessage.getRemoteIp(), 1235);
                            pDeviceEcho->loadXML("Config/devices/");

                            // Add device to list
                            pDeviceManager->addDevice(pDeviceEcho);
                            
                            // TEMP / TODO : attach to surface main
                            if (Globals::instance()->mp_app->mp_surfaceMain)
                                Globals::instance()->mp_app->mp_surfaceMain->addDevice(pDeviceEcho);

                            // Create a node in visualisation scene
                            Globals::instance()->mp_app->createDeviceNodeInScene(pDeviceEcho);

                            // Update interface
                         
							//Globals::instance()->mp_app->guiMarkUpdateLayout();
                            // Globals::instance()->mp_app->guiUpdateListDevices();
                            //Globals::instance()->mp_app->selectDevice(pDeviceEcho->m_id);
                        
                            // TODO : Send configuration parameters to it (if any)
                        }
                    }
                    
                    // Decode new message
                    if (pDevice)
                    {
                        // pDevice->onReceivePacketBegin();
                        
                        int nbPackets = (m_oscMessage.getNumArgs() - 1)/4; // TODO : Be careful with this
                        for (int i=0;i<nbPackets;i++)
                        {
                            m_packetTemp.m_volume = m_oscMessage.getArgAsFloat(indexArg++);
                            // TODO : Other arguments to decode here
//							if (m_oscMessage.getNumArgs()>=3)
							{
								 m_packetTemp.m_color.setHue( m_oscMessage.getArgAsFloat(indexArg++) );
								 m_packetTemp.m_color.setSaturation( m_oscMessage.getArgAsFloat(indexArg++) );
								 m_packetTemp.m_color.setBrightness( m_oscMessage.getArgAsFloat(indexArg++) );
							}
							
                            // Send to device
                            pDevice->onReceivePacket(&m_packetTemp);

                        }

                        //pDevice->onReceivePacketEnd();

                        // Get the surface of device and update animation
                        Surface* pSurface = Globals::instance()->mp_app->getSurfaceForDevice(pDevice);
                        
                        if (pSurface){
                            pSurface->onNewPacket( pDevice->getLastPacket(), pDevice->m_id );
                        }
                    
                    }
                }
            }
            
            
            
            // New Device plugged
            // printf("osc %s - %d arguments\n", m_oscMessage.getAddress().c_str(), m_oscMessage.getNumArgs());
            
            // New Device event
        }
    }
}
