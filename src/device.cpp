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
#include "ofAppLog.h"
#include "globals.h"

//--------------------------------------------------------------
DevicePacket::DevicePacket()
{
    m_volume = 0.0f;
}

//--------------------------------------------------------------
void DevicePacket::copy(DevicePacket* pPacket)
{
    m_volume = pPacket->m_volume;
	m_color = pPacket->m_color;
}

//--------------------------------------------------------------
void DevicePacket::computeColor(const ofColor& deviceColor)
{
	m_color.setHue(deviceColor.getHue());
	m_color.setSaturation(deviceColor.getSaturation());
	m_color.setBrightness(m_volume*255.0f);
}

//--------------------------------------------------------------
void DevicePacket::computeColor(const float* deviceColor)
{
	m_color = ofColor::fromHsb(deviceColor[0],deviceColor[1],m_volume*255.0f);
//	m_color.setHsb(deviceColor[0],deviceColor[1],m_volume*255.0f);
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
	mp_sampleStandBy = 0;
	m_sampleVolStandby = 0.35f;
	m_sampleNameStandby = "Sounds/StandBy/theme1-4.wav";

	m_isEnableStandup	= false;
	m_standupTh			= 0.55f;
	
	m_color.setHsb(200, 255, 255); // Brightness not used as set by packets
	m_colorMode = colorMode_manual_hsb;
	m_colorSpeedOscillation = 10.0f;
}

//--------------------------------------------------------------
Device::~Device()
{
	Sample::lock();
	delete mp_sampleStandBy;
	mp_sampleStandBy = 0;
	if (mp_soundInput) mp_soundInput->setSample(0);
	Sample::unlock();

    delete mp_soundInput;
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
}

//--------------------------------------------------------------
void Device::setPointSurface(float xNorm, float yNorm)
{
    m_pointSurface.set(xNorm,yNorm);
}

//--------------------------------------------------------------
void Device::setColorHueSaturation(float h, float s)
{
	m_colorHsv[0]=h;
	m_colorHsv[1]=s;

	m_color.setHue(h);
	m_color.setSaturation(s);

    ofxOscMessage m;
	m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("color");
	m.addFloatArg(m_colorHsv[0]);
	m.addFloatArg(m_colorHsv[1]);
    m_oscSender.sendMessage(m);
}

void Device::setColorHue(float h)
{
	//OFAPPLOG->begin("Device::setColorHue("+ofToString(h)+")");

	m_color.setHue(h);
	m_colorHsv[0]=h;

	// OFAPPLOG->println("m_color.getHue()="+ofToString(m_color.getHue()));
	// OFAPPLOG->println("m_color.getSaturation()="+ofToString(m_color.getSaturation()));

    ofxOscMessage m;
	m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("color");
	m.addFloatArg(m_colorHsv[0]);
	m.addFloatArg(m_colorHsv[1]);
    m_oscSender.sendMessage(m);

	//OFAPPLOG->end();
}

void Device::setColorSaturation(float s)
{
	//OFAPPLOG->begin("Device::setColorSaturation("+ofToString(s)+")");


	m_color.setSaturation(s);
	m_colorHsv[1]=s;

	//OFAPPLOG->println("m_color.getHue()="+ofToString(m_color.getHue()));
	//OFAPPLOG->println("m_color.getSaturation()="+ofToString(m_color.getSaturation()));


    ofxOscMessage m;
	m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("color");
	m.addFloatArg(m_colorHsv[0]);
	m.addFloatArg(m_colorHsv[1]);
    m_oscSender.sendMessage(m);
	
	//OFAPPLOG->end();
}

//--------------------------------------------------------------
void Device::setColorHueSaturationOSC(float h, float s)
{
	m_colorHsv[0]=h;
	m_colorHsv[1]=s;

	m_color.setHue(h);
	m_color.setSaturation(h);
	
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
    // printf("setVolumeMax, v=%.3f\n", v);
    ofxOscMessage m;
	m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("volMax");
    m.addFloatArg(v);
    m_oscSender.sendMessage(m);

	LOG_MESSAGE_OSC(m,false);

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

	LOG_MESSAGE_OSC(m,false);

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

	LOG_MESSAGE_OSC(m,false);
 
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

	LOG_MESSAGE_OSC(m,false);

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
void Device::resetStandBy()
{
	if (mp_soundInput==0) return;

	Sample::lock();

	if (mp_sampleStandBy)
	{
		delete mp_sampleStandBy;
		mp_sampleStandBy = 0;
	}

	mp_sampleStandBy = new Sample();
	if ( mp_sampleStandBy->load(ofToDataPath(m_sampleNameStandby)) )
	{
		mp_soundInput->setSample(mp_sampleStandBy);
	}else
	{
		delete mp_sampleStandBy;
		mp_sampleStandBy = 0;
		mp_soundInput->setSample(0);
	}
	Sample::unlock();
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

	LOG_MESSAGE_OSC(m,false);
 
    m_timeStandby = v;
}

//--------------------------------------------------------------
void Device::setTimeStandbyOSC(float v)
{
    m_timeStandby = v;
}

//--------------------------------------------------------------
void Device::setSampleVolumeStandby(float v)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("sampleVolStandby");
    m.addFloatArg(v);
    m_oscSender.sendMessage(m);

	LOG_MESSAGE_OSC(m,false);
	
	m_sampleVolStandby = v;
}

//--------------------------------------------------------------
void Device::setSampleVolumeStandbyOSC(float v)
{
    if (mp_soundInput){
        mp_soundInput->setSampleVolume(v);
    }
}

//--------------------------------------------------------------
void Device::setEnableStandup(bool is)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("enableStandup");
    m.addIntArg(is ? 1 : 0);
    m_oscSender.sendMessage(m);

	LOG_MESSAGE_OSC(m,false);
	
	m_isEnableStandup = is;
}

//--------------------------------------------------------------
void Device::setEnableStandupOSC(bool is)
{
	m_isEnableStandup = is;
}


//--------------------------------------------------------------
void Device::setStandupVol(float v)
{
    ofxOscMessage m;
    m.setAddress( OSC_ADDRESS_SET_DEVICE_PROP );
    m.addStringArg(m_id);
    m.addStringArg("volStandup");
    m.addFloatArg(v);
    m_oscSender.sendMessage(m);
	
	LOG_MESSAGE_OSC(m,false);
	
	m_standupTh = v;
}

//--------------------------------------------------------------
void Device::setStandupVolOSC(float v)
{
	m_standupTh = v;
}



//--------------------------------------------------------------
void Device::drawSoundInputVolume(float x, float y)
{
    if (mp_soundInput){
        mp_soundInput->drawVolume(x,y);
        ofSetColor(255);

        if (m_stateStandby == EStandby_active)
            m_stateStandbyStr = "active";
        else if (m_stateStandby == EStandby_pre_standby)
            m_stateStandbyStr = "standby (pre)";
        else if (m_stateStandby == EStandby_standby)
            m_stateStandbyStr = "standby";
        else if (m_stateStandby == EStandby_standup){
            m_stateStandbyStr = "standup";
        }
     
        ofDrawBitmapString(m_stateStandbyStr, x-getWidthSoundInputVolume()/2,y+12);
        ofSetColor(0,255,0);
        float yTh = y+(1.0-m_volHistoryTh)*getHeightSoundInputVolume();
        ofLine(x-getWidthSoundInputVolume()/2,yTh,x,yTh);

        ofSetColor(0,0,255);
		yTh = y+(1.0-m_standupTh)*getHeightSoundInputVolume();
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
        mp_soundInput->setup(nbChannels); // do not create an audio stream
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
//		m_listPackets[i]->computeColor(m_color);
		m_listPackets[i]->computeColor(m_colorHsv);

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
		m.addFloatArg(m_listPackets[i]->m_color.getHue());
		m.addFloatArg(m_listPackets[i]->m_color.getSaturation());
		m.addFloatArg(m_listPackets[i]->m_color.getBrightness());
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
	if (mp_soundInput == 0) return;


    if (m_isEnableStandbyMode)
    {
        m_stateStandbyDuration += dt;
        
        if (m_stateStandby == EStandby_active)
        {
		   if (mp_soundInput->getVolHistoryMeanFiltered() <= m_volHistoryTh)
		   {
			   m_stateStandby = EStandby_pre_standby;
			   m_stateStandbyDuration=0.0f;
		   }
		   else if (mp_soundInput->getVolHistoryMeanFiltered() >= m_standupTh)
		   {
			   if (m_isEnableStandup)
				   m_stateStandby = EStandby_standup;
		   }
        }
        else
        if (m_stateStandby == EStandby_standup)
        {
		   if (mp_soundInput->getVolHistoryMeanFiltered() < m_standupTh)
		   {
			   m_stateStandby = EStandby_active;
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
			else
			{
	            if (m_stateStandbyDuration>=m_timeStandby)
    	        {
        	        m_stateStandby = EStandby_standby;
            	    m_stateStandbyDuration=0.0f;
					resetStandBy();
                // ofLog() << "> Entering standby\n";
            	}
        	}
		}
        else
        if (m_stateStandby == EStandby_standby)
        {
			Sample::lock();
			bool isSampleFinished = false;
			if (mp_sampleStandBy && !mp_sampleStandBy->getIsPlaying())
				isSampleFinished = true;
		
            if (isSampleFinished || mp_soundInput->getVolHistoryMeanFiltered() > m_volHistoryTh)
            {
                m_stateStandby = EStandby_pre_standby;
                m_stateStandbyDuration=0.0f;
				
				mp_soundInput->setSample(0);

            }
			Sample::unlock();
        }
    }
    else
	{
		if (m_stateStandby == EStandby_active)
		{
			if (mp_soundInput->getVolHistoryMeanFiltered() >= this->m_standupTh)
			{
				if (m_isEnableStandup)
		        	m_stateStandby = EStandby_standup;
			}
		}
		else if (m_stateStandby == EStandby_standup)
		{
			if (mp_soundInput->getVolHistoryMeanFiltered() < this->m_standupTh)
			{
		        m_stateStandby = EStandby_active;
			}
		}

	
	}
}

//--------------------------------------------------------------
void Device::update(float dt)
{
    checkForActivity(dt);
    
    if (mp_soundInput)
    {
        // Update sound data
        mp_soundInput->update();

		// Sample them and writes into packet
        sampleSoundInput();

        // Send packets / values to network
        sendPacketsOSC();
    }
}

//--------------------------------------------------------------
void Device::loadXML(string dir)
{
	OFAPPLOG->begin("Device::loadXML()");

    ofxXmlSettings settings;
    string pathFile = getPathXML(dir);
    if ( settings.loadFile(pathFile) )
    {
        string surfaceId = settings.getAttribute("device:surface", "id", "main");
        float xNorm = settings.getValue("device:surface:xNorm", 0.5f);
        float yNorm = settings.getValue("device:surface:yNorm", 0.5f);
		
		float 	volMax 				= settings.getValue("device:soundInput:volMax",0.05f);
		int 	volHistoryNb 		= settings.getValue("device:soundInput:volHistoryNb", 400);
		float 	volHistoryTh		= settings.getValue("device:soundInput:volHistoryTh",0.1f);
     	int		enableStandby		= settings.getValue("device:enableStandby",1);
	 	float 	timeStandby			= settings.getValue("device:timeStandby",10.0f);
	 	float 	sampleVolStandby	= settings.getValue("device:sampleVolStandby",0.35f);
     	int		enableStandup		= settings.getValue("device:enableStandup",1);
	 	float 	volStandup			= settings.getValue("device:volStandup",0.5f);

		float	colorManualHsb[2];
		colorManualHsb[0]			= settings.getValue("device:color:colorMode_manual_hsb:hue", 		127.0f);
		colorManualHsb[1]			= settings.getValue("device:color:colorMode_manual_hsb:saturation", 	127.0f);
	 
		OFAPPLOG->println("Device, loaded "+pathFile);
		OFAPPLOG->println(" - volMax="+ofToString(volMax));
		OFAPPLOG->println(" - volHistoryNb="+ofToString(volHistoryNb));
		OFAPPLOG->println(" - volHistoryTh="+ofToString(volHistoryTh));
		OFAPPLOG->println(" - enableStandby="+ofToString(timeStandby));
		OFAPPLOG->println(" - timeStandby="+ofToString(enableStandby));
		OFAPPLOG->println(" - sampleVolStandby="+ofToString(sampleVolStandby));
		OFAPPLOG->println(" - enableStandup="+ofToString(enableStandup));
		OFAPPLOG->println(" - volStandup="+ofToString(volStandup));
		OFAPPLOG->println(" - surface="+surfaceId+" (xNorm="+ofToString(xNorm)+",yNorm="+ofToString(yNorm)+")");
		OFAPPLOG->println(" - colorManualHsb, (hue="+ofToString(colorManualHsb[0])+", saturation="+ofToString(colorManualHsb[1])+")");
	 
        setSoundInputVolumeMax( volMax );
        setSoundInputVolHistorySize( volHistoryNb );
        setSoundInputVolHistoryTh( volHistoryTh );
        setEnableStandbyMode( enableStandby == 1 ? true : false );
        setTimeStandby( timeStandby );
        setSampleVolumeStandby( sampleVolStandby );
		// setEnableStandup( enableStandup );
		setStandupVol( volStandup );
        setPointSurface(xNorm, yNorm);
		setColorHueSaturation(colorManualHsb[0],colorManualHsb[1]);

		settings.pushTag("device");
		settings.pushTag("soundOutput");
		int nbSpeakers = settings.getNumTags("speaker");
		clearListSpeakers();
		for (int i=0;i<nbSpeakers;i++){
			int speakerId = settings.getValue("speaker",0,i);
			addSpeakerId( speakerId );
	        OFAPPLOG->println(" - speaker "+ofToString(speakerId)+" added");
		}
		settings.popTag();
		settings.popTag();

     
    }
    else{
        OFAPPLOG->println("Device, error loading "+pathFile);
    }


	OFAPPLOG->end();
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

        settings.addTag("soundOutput");
        settings.pushTag("soundOutput");
		for (int i=0; i<m_listSpeakerIds.size(); i++)
            settings.addValue("speaker", m_listSpeakerIds[i]);
        settings.popTag();

        settings.addTag("color");
        settings.pushTag("color");
			settings.addTag("colorMode_manual_hsb");
			settings.pushTag("colorMode_manual_hsb");
				settings.addValue("hue", 			m_colorHsv[0]);
				settings.addValue("saturation", 	m_colorHsv[1]);
			settings.popTag();
        settings.popTag();

    settings.addValue("enableStandby", 		getEnableStandbyMode() ? 1 : 0);
    settings.addValue("timeStandby", 		m_timeStandby);
    settings.addValue("sampleVolStandby", 	getSampleVolStandby());
    settings.addValue("enableStandby", 		getEnableStandup() ? 1 : 0);
/*
    settings.addValue("enableStandup", 		getEnableStandup() ? 1 : 0);
    settings.addValue("volStandup", 		getStandupVol());
*/
    settings.addTag("surface");
    settings.setAttribute("surface", "id", "main", 0);
    settings.pushTag("surface");
        settings.addValue("xNorm", 			m_pointSurface.x);
        settings.addValue("yNorm", 			m_pointSurface.y);
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
void Device::clearListSpeakers()
{
	m_listSpeakerIds.clear();
}

//--------------------------------------------------------------
void Device::addSpeakerId(int id)
{
	m_listSpeakerIds.push_back(id);
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

//--------------------------------------------------------------
int DeviceManager::indexOf(Device* pDevice)
{
	int index=0;
	for (itDevices = m_listDevices.begin(); itDevices != m_listDevices.end(); ++itDevices)
	{
		if (*itDevices == pDevice)
			return index;
		index++;
	}
	return -1;
}







