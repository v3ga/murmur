/*
 *  globals.cpp
 *  versatubes
 *
 *  Created by 2Roqs on 01/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#include "globals.h"
#include "testApp.h"
#include "device.h"


//--------------------------------------------------------------
Globals* Globals::smp_instance = 0;

//--------------------------------------------------------------
Globals::Globals()
{
	mp_timeline = 0;
	mp_app = 0;
    mp_oscReceiver = 0;
    mp_oscSender = 0;
    mp_deviceManager = 0;
	m_isAutoReloadScript = false;
}

//--------------------------------------------------------------
Globals* Globals::instance()
{
	if (smp_instance == 0)
		smp_instance = new Globals();
	return smp_instance; 
}

//--------------------------------------------------------------
AnimationManager* Globals::getAnimationManagerForDevice(string deviceId)
{
	if (mp_app)
		return mp_app->getAnimationManagerForDevice(deviceId);
	return 0;
}

