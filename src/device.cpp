/*
 *  device.cpp
 *  murmur
 *
 *  Created by Julien on 10/04/13.
 *  Copyright 2013 2Roqs. All rights reserved.
 *
 */

#include "device.h"
#include "soundInput.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
DevicePacket::DevicePacket()
{
    m_volume = 0.0f;
}

//--------------------------------------------------------------
void DevicePacket::copy(DevicePacket* pPacket)
{
    m_volume = pPacket->m_volume;
}

//--------------------------------------------------------------
Device::Device(string id, int nbLEDs, float distLEDs)
{
    mp_soundInput = 0;
    m_indexPacketReceived = 0;
    setPointSurface(0.5f,0.5f);
    set(id,nbLEDs,distLEDs);
    m_isActive = false;
    m_isEnableStandbyMode = true;
    m_stateStandby = EStandby_active;
    m_stateStandbyDuration = 0.0f;
    m_timeStandby = 10.0f;
    m_nbLEDsStandby = 50;
    mp_sampleStandBy = 0;
    m_angleSampleStandby = 0.0f;
    m_angleSampleStandbySpeed = 70.0f;
    
    mp_sampleStandBy = new float[m_nbLEDs];
    for (int i=0;i<m_nbLEDs;i++){
        mp_sampleStandBy[i]=0.0f;
    }

}

//--------------------------------------------------------------
Device::~Device()
{
    delete mp_soundInput;
    delete[] mp_sampleStandBy;
}

//--------------------------------------------------------------
void Device::set(string id, int nbLEDs, float distLEDs)
{
    if (nbLEDs<2)
        nbLEDs = 2;
    
    m_id = id;
    m_nbLEDs = nbLEDs;
    m_distLEDs = distLEDs;
    m_length = m_distLEDs*(m_nbLEDs-1);
//    printf("m_length=%.3f\n", m_length);
    
    createPackets(m_nbLEDs);

    mp_sampleStandBy = new float[m_nbLEDs];
    for (int i=0;i<m_nbLEDs;i++){
        mp_sampleStandBy[i]=0.0f;
    }
}

//--------------------------------------------------------------
void Device::setPointSurface(float xNorm, float yNorm)
{
    m_pointSurface.set(xNorm,yNorm);
}


//--------------------------------------------------------------
float Device::getWidthSoundInputVolume()
{
    if (mp_soundInput)
        return (float)2*mp_soundInput->getVolHistory().size();
    return 0.0f;
}

//--------------------------------------------------------------
float Device::getHeightSoundInputVolume()
{
    if (mp_soundInput)
        return (float)mp_soundInput->getHeightDraw();
    return 0.0f;
}


//--------------------------------------------------------------
void Device::setSoundInputVolumeMax(float v)
{
    //printf("setVolumeMax, v=%.3f\n", v);
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("volMax");
    m.addFloatArg(v);
    m_oscSender.sendMessage(m);

    m_soundInputVolEmpiricalMax = v;
}

//--------------------------------------------------------------
void Device::setSoundInputVolHistorySize(int nb)
{
    //printf("setSoundInputVolHistorySize, v=%d\n", nb);
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("volHistorySize");
    m.addIntArg(nb);
    m_oscSender.sendMessage(m);

    m_soundInputVolHistorySize = nb;
}


//--------------------------------------------------------------
void Device::setSoundInputVolHistorySizeOSC(int nb)
{
    if (mp_soundInput){
        mp_soundInput->setVolHistorySize(nb);
    }
}


//--------------------------------------------------------------
void Device::setSoundInputVolumeMaxOSC(float v)
{
    //printf("setVolumeMaxOSC, v=%.3f\n", v);
    if (mp_soundInput){
        mp_soundInput->setVolEmpiricalMax(v);
    }
}


//--------------------------------------------------------------
void Device::setSoundInputVolHistoryTh(float th)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("volHistoryTh");
    m.addFloatArg(th);
    m_oscSender.sendMessage(m);
    
    m_volHistoryTh = th;
}

//--------------------------------------------------------------
void Device::setSoundInputVolHistoryThOSC(float th)
{
    m_volHistoryTh = th;
}

//--------------------------------------------------------------
void Device::turnoff()
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_TURN_OFF );
    m.addStringArg(m_id);
    m_oscSender.sendMessage(m);
}

//--------------------------------------------------------------
void Device::setEnableStandbyMode(bool is)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("enableStandbyMode");
    m.addIntArg(is ? 1 : 0);
    m_oscSender.sendMessage(m);

    enableStandbyMode(is);
}

//--------------------------------------------------------------
void Device::setEnableStandbyModeOSC(bool is)
{
    enableStandbyMode(is);
}

//--------------------------------------------------------------
float Device::getSoundInputVolumeMax()
{
//    if (mp_soundInput)
//        return mp_soundInput->getVolEmpiricalMax();
    return m_soundInputVolEmpiricalMax;
}

//--------------------------------------------------------------
float Device::getSoundInputVolHistorySize()
{
//    if (mp_soundInput)
//        return mp_soundInput->getVolHistory().size();
    return m_soundInputVolHistorySize;
}

//--------------------------------------------------------------
void Device::setTimeStandby(float v)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("timeStandby");
    m.addFloatArg(v);
    m_oscSender.sendMessage(m);
    
    m_timeStandby = v;
}

//--------------------------------------------------------------
void Device::setTimeStandbyOSC(float v)
{
    m_timeStandby = v;
}

//--------------------------------------------------------------
void Device::setNbLEDsStandby(int nb)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("nbLEDsStandby");
    m.addIntArg(nb);
    m_oscSender.sendMessage(m);

    m_nbLEDsStandby = nb;
}

//--------------------------------------------------------------
void Device::setNbLEDsStandbyOSC(int nb)
{
    m_nbLEDsStandby = nb;
}

//--------------------------------------------------------------
void Device::setSpeedStandby(float v)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("speedStandby");
    m.addFloatArg(v);
    m_oscSender.sendMessage(m);
    
    m_angleSampleStandbySpeed = v;
}

//--------------------------------------------------------------
void Device::setSpeedStandbyOSC(float v)
{
    m_angleSampleStandbySpeed = v;
}


//--------------------------------------------------------------
void Device::drawSoundInputVolume(float x, float y)
{
    if (mp_soundInput){
        mp_soundInput->drawVolume(x,y);
        ofSetColor(255);

//        printf("%.2f / %.2f\n", m_stateStandbyDuration, m_timeStandby);;
        if (m_stateStandby == EStandby_active)
            m_stateStandbyStr = "active";
        else if (m_stateStandby == EStandby_pre_standby)
            m_stateStandbyStr = "standby (pre)";
        else if (m_stateStandby == EStandby_standby){
            m_stateStandbyStr = "standby ";
        }
        
        ofDrawBitmapString(m_stateStandbyStr, x-getWidthSoundInputVolume()/2,y+12);
        ofSetColor(0,255,0);
        float yTh = y+(1.0-m_volHistoryTh)*getHeightSoundInputVolume();
        ofLine(x-getWidthSoundInputVolume()/2,yTh,x,yTh);
    }
}

//--------------------------------------------------------------
void Device::startSoundInput(int deviceId, int nbChannels)
{
    if (mp_soundInput==0)
    {
        // Create sound input
        mp_soundInput = new SoundInput();
        mp_soundInput->setup(deviceId,nbChannels);
    }
}

//--------------------------------------------------------------
void Device::startSoundInput(int nbChannels)
{
    if (mp_soundInput==0)
    {
        // Create sound input
        mp_soundInput = new SoundInput();
        mp_soundInput->setup(nbChannels);
    }
}



//--------------------------------------------------------------
void Device::audioIn(float * input, int bufferSize, int nChannels)
{
    if (mp_soundInput){
		mp_soundInput->audioIn(input, bufferSize, nChannels);
	}
}

//--------------------------------------------------------------
void Device::createPackets(int nb)
{
    deletePackets();
    for (int i=0;i<nb;i++){
        m_listPackets.push_back( new DevicePacket() );
    }
}

//--------------------------------------------------------------
void Device::deletePackets()
{
    vector<DevicePacket*>::iterator it;
    for(it = m_listPackets.begin(); it != m_listPackets.end();)
    {
        delete *it;
        it = m_listPackets.erase(it);
    }
}



//--------------------------------------------------------------
void Device::sampleStandbyInput()
{
    if (m_listPackets.size()<=2) return;

    int nbPackets = m_listPackets.size();
    for (int i=0;i<nbPackets;i++)
        m_listPackets[i]->m_volume = mp_sampleStandBy[i];
}

//--------------------------------------------------------------
void Device::generateSampleStandBy(float dt)
{
    float step = PI / float(m_nbLEDsStandby-1);
    for (int i=0;i<m_nbLEDs;i++)
    {
        if (i<=m_nbLEDsStandby){
            mp_sampleStandBy[i] = 0.5f * (1.0f+sin( float(i)*step-ofDegToRad(m_angleSampleStandby) ));
        }
        else{
            mp_sampleStandBy[i] = 0.0f;
        }
    }
//    printf("%.2f -", m_angleSampleStandby);
    m_angleSampleStandby += m_angleSampleStandbySpeed*dt;
    if (m_angleSampleStandby>=360.0f)
        m_angleSampleStandby -= 360.0f;
}

//--------------------------------------------------------------
void Device::sampleSoundInput()
{
    if (!mp_soundInput) return;
    if (m_listPackets.size()<=2) return;
    
    int nbPackets = m_listPackets.size();
    int nbVolHistorySize = mp_soundInput->getVolHistory().size();
    float stepSample = float( mp_soundInput->getVolHistory().size() )/ float(nbPackets-1) ;
    
    float indexSample = 0.0f;
    for (int i=0;i<nbPackets;i++)
    {
        if (indexSample>nbVolHistorySize-1)
            indexSample = nbVolHistorySize-1;
        
        m_listPackets[i]->m_volume = mp_soundInput->getVolHistory()[(int)indexSample]; // Nearest sampling

        indexSample += stepSample;
    }
}

//--------------------------------------------------------------
void Device::sendPacketsOSC()
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SEND_PACKETS );
    m.addStringArg(m_id);

    int nbPackets = m_listPackets.size();
    for (int i=0;i<nbPackets;i++)
    {
		m.addFloatArg(m_listPackets[i]->m_volume);
    }
    m_oscSender.sendMessage(m);
}

//--------------------------------------------------------------
void Device::onReceivePacketBegin()
{
    m_indexPacketReceived = 0;
}


//--------------------------------------------------------------
void Device::onReceivePacket(DevicePacket* pPacket)
{
    if (m_indexPacketReceived<m_listPackets.size())
    {
        m_listPackets[m_indexPacketReceived]->copy(pPacket);
        m_indexPacketReceived++;
    }
}

//--------------------------------------------------------------
void Device::onReceivePacketEnd()
{
}


//--------------------------------------------------------------
void Device::enableStandbyMode(bool is)
{
    m_isEnableStandbyMode = is;
    if (is){
        m_stateStandbyDuration=0.0f;
    }
}

//--------------------------------------------------------------
void Device::checkForActivity(float dt)
{
    if (m_isEnableStandbyMode)
    {
        m_stateStandbyDuration += dt;
        
        if (m_stateStandby == EStandby_active)
        {
            if (mp_soundInput && (mp_soundInput->getVolHistoryMeanFiltered() <= m_volHistoryTh))
            {
                m_stateStandby = EStandby_pre_standby;
                m_stateStandbyDuration=0.0f;
            }
        }
        else
        if (m_stateStandby == EStandby_pre_standby)
        {
            //printf("%.2f - ",m_stateStandbyDuration);
            if (mp_soundInput && (mp_soundInput->getVolHistoryMeanFiltered() > m_volHistoryTh))
            {
                m_stateStandby = EStandby_active;
                m_stateStandbyDuration=0.0f;
            }

            if (m_stateStandbyDuration>=m_timeStandby)
            {
                m_stateStandby = EStandby_standby;
                m_stateStandbyDuration=0.0f;
                printf("> Entering standby\n");
            }

        }
        else
        if (m_stateStandby == EStandby_standby)
        {
            if (mp_soundInput && (mp_soundInput->getVolHistoryMeanFiltered() > m_volHistoryTh))
            {
                m_stateStandby = EStandby_active;
                m_stateStandbyDuration=0.0f;
            }
        }
    }
    else
        m_stateStandby = EStandby_active;
}

//--------------------------------------------------------------
void Device::update(float dt)
{
    checkForActivity(dt);
    
    if (mp_soundInput)
    {
        // Update sound data
        mp_soundInput->update();

        // If stand by, send our own packets
        if (m_stateStandby == EStandby_standby)
        {
            generateSampleStandBy(dt);
            sampleStandbyInput();
        }
        // sample
        else
        {
            // Sample them and writes into packet
            sampleSoundInput();
        }
        

        // Send packets / values to network
        sendPacketsOSC();
    }
}

//--------------------------------------------------------------
void Device::loadXML(string dir)
{
    ofxXmlSettings settings;
    string pathFile = getPathXML(dir);
    if ( settings.loadFile(pathFile) )
    {
        string surfaceId = settings.getAttribute("device:surface", "id", "main");
        float xNorm = settings.getValue("device:surface:xNorm", 0.5f);
        float yNorm = settings.getValue("device:surface:yNorm", 0.5f);
        
        
        printf("Device, loaded %s\n", pathFile.c_str());
        printf("    - volMax=%.3f\n", settings.getValue("device:soundInput:volMax",0.05f));
        printf("    - volHistoryNb=%d\n", settings.getValue("device:soundInput:volHistoryNb",400));
        printf("    - volHistoryTh=%.3f\n", settings.getValue("device:soundInput:volHistoryTh",0.1f));
        printf("    - enableStandby=%s\n", settings.getValue("device:enableStandby",1) ? "true" : "false");
        printf("    - timeStandby=%.2f\n", settings.getValue("device:timeStandby",10.0f) );
        printf("    - nbLEDsStandby=%d\n", settings.getValue("device:nbLEDsStandby",50) );
        printf("    - speedStandby=%.2f\n", settings.getValue("device:speedStandby",70.0f) );
        printf("    - surface=%s (xNorm=%.2f,yNorm=%.2f)\n",surfaceId.c_str(), xNorm, yNorm);
        
        setSoundInputVolumeMax( settings.getValue("device:soundInput:volMax",0.05f) );
        setSoundInputVolHistorySize( settings.getValue("device:soundInput:volHistoryNb",400) );
        setSoundInputVolHistoryTh( settings.getValue("device:soundInput:volHistoryTh",0.1f) );
        setEnableStandbyMode( settings.getValue("device:enableStandby",1)==1 ? true : false );
        setTimeStandby( settings.getValue("device:timeStandby",10.0f) );
        setNbLEDsStandby( settings.getValue("device:nbLEDsStandby", 50) );
        setSpeedStandbyOSC( settings.getValue("device:speedStandby", 70.0) );
        setPointSurface(xNorm, yNorm);
        
    }
    else{
        printf("Device, error loading %s\n", pathFile.c_str());
    }
}


//--------------------------------------------------------------
void Device::saveXML(string dir)
{
    ofxXmlSettings settings;
    settings.addTag("device");
    settings.setAttribute("device", "id", m_id,0);
    settings.pushTag("device");

        settings.addTag("soundInput");
        settings.pushTag("soundInput");
            settings.addValue("volMax", getSoundInputVolumeMax());
            settings.addValue("volHistoryNb", getSoundInputVolHistorySize());
            settings.addValue("volHistoryTh", getSoundInputVolHistoryTh());
        settings.popTag();
    settings.addValue("enableStandby", getEnableStandbyMode() ? 1 : 0);
    settings.addValue("timeStandby", m_timeStandby);
    settings.addValue("nbLEDsStandby", getNbLEDsStandby());
    settings.addValue("speedStandby", getSpeedStandby());
    
    settings.addTag("surface");
    settings.setAttribute("surface", "id", "main", 0);
    settings.pushTag("surface");
        settings.addValue("xNorm", m_pointSurface.x);
        settings.addValue("yNorm", m_pointSurface.y);
    settings.popTag();
   
    
    settings.popTag();
    settings.saveFile( getPathXML(dir) );
    
}

//--------------------------------------------------------------
string Device::getPathXML(string dir)
{
    string path = dir + m_id + ".xml";
    return path;
}


//--------------------------------------------------------------
DeviceManager::DeviceManager()
{
    mp_deviceCurrent = 0;
}

//--------------------------------------------------------------
DeviceManager::~DeviceManager()
{
    deleteDevices();
}

//--------------------------------------------------------------
void DeviceManager::deleteDevices()
{
	for (itDevices = m_listDevices.begin(); itDevices != m_listDevices.end();)
    {
        delete *itDevices;
        itDevices = m_listDevices.erase(itDevices);
    }
}

//--------------------------------------------------------------
void DeviceManager::addDevice(Device* pDevice)
{
    m_listDevices.push_back( pDevice );
}


//--------------------------------------------------------------
Device* DeviceManager::getDeviceById(string id)
{
    Device* pDevice=0;
	for (itDevices = m_listDevices.begin(); itDevices != m_listDevices.end(); ++itDevices){
        if ( (*itDevices)->m_id == id ){
            pDevice = *itDevices;
            break;
        }
    }
    return pDevice;
}

//--------------------------------------------------------------
void DeviceManager::getDevicesListId(vector<string>& listIds)
{
	for (itDevices = m_listDevices.begin(); itDevices != m_listDevices.end(); ++itDevices){
        listIds.push_back( (*itDevices)->m_id );
    }
}

//--------------------------------------------------------------
void DeviceManager::setDeviceCurrent(Device* pDevice)
{
    mp_deviceCurrent = pDevice;
}

//--------------------------------------------------------------
Device* DeviceManager::setDeviceCurrent(string deviceId)
{
    Device* pDevice = getDeviceById(deviceId);
    setDeviceCurrent( pDevice );
    return pDevice;
}

//--------------------------------------------------------------
void DeviceManager::saveDevicesXML(string dir)
{
	for (itDevices = m_listDevices.begin(); itDevices != m_listDevices.end(); ++itDevices)
        (*itDevices)->saveXML(dir);
}

//--------------------------------------------------------------
void DeviceManager::turnoffDevices()
{
	for (itDevices = m_listDevices.begin(); itDevices != m_listDevices.end(); ++itDevices)
        (*itDevices)->turnoff();
}




