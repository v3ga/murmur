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
                    if (propName == "nbLEDsStandby")
                    {
                        // pDevice->setNbLEDsStandbyOSC( m_oscMessage.getArgAsInt32(indexArg) );
                    }
                    else
                    if (propName == "speedStandby")
                    {
                        // pDevice->setSpeedStandbyOSC( m_oscMessage.getArgAsFloat(indexArg) );
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

                    // Not found, create the device now
                    if (pDevice == 0)
                    {
                        // TODO : type ? -> factory
                        if (Globals::instance()->mp_app->isSimulation == false)
                        {
                            int nbLEDs = m_oscMessage.getNumArgs() - indexArg; // WARNING with this
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
                            Globals::instance()->mp_app->guiUpdateListDevices();
                            Globals::instance()->mp_app->selectDevice(pDeviceEcho->m_id);
                        
                            // TODO : Send configuration parameters to it (if any)
                        }
                    }
                    
                    // Decode new message
                    if (pDevice)
                    {
                        pDevice->onReceivePacketBegin();
                        
                        int nbPackets = m_oscMessage.getNumArgs() - indexArg; // TODO : Be careful with this
                        for (int i=0;i<nbPackets;i++)
                        {
                            m_packetTemp.m_volume = m_oscMessage.getArgAsFloat(indexArg++);
                            // TODO : Other arguments to decode here
                            
                            // Send to device
                            pDevice->onReceivePacket(&m_packetTemp);

                        }

                        pDevice->onReceivePacketEnd();

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
