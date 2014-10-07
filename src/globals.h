/*
 *  globals.h
 *  versatubes
 *
 *  Created by 2Roqs on 01/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#pragma once

#include "oscReceiver.h"
#include "oscSender.h"
#include "animations.h"
#include "timeline.h"
#include "ofAppLog.h"

#define GLOBALS Globals::instance()
#define LOG_MESSAGE_OSC(message,isIn) GLOBALS->logMessageOSC(message,isIn)

class testApp;
class DeviceManager;
class DeviceInfoManager;
class Globals
{
	public:
		Globals					();


		static Globals*			instance();
		oscReceiver*			mp_oscReceiver;
        oscSender*              mp_oscSender;
		Timeline*				mp_timeline;
		testApp*				mp_app;
        DeviceManager*          mp_deviceManager;
        bool					m_isAutoReloadScript;

		void					logMessageOSC				(const ofxOscMessage&, bool isIn=true);

		Surface*				getSurfaceMain();
		AnimationManager*		getAnimationManagerForDevice(string deviceId);
		Animation*				getAnimationCurrentForDevice(string deviceId);

	private:
		static Globals*			smp_instance;
};
