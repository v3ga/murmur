//
//  toolDevices.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once

#include "tool.h"
#include "device.h"
#include "midiInterface.h"
#include "classProperty.h"


class ofxUIMovingGraphThreshold;
class toolDevices : public tool, public midiInterface
{
	public:

		toolDevices				(toolManager* parent, DeviceManager*);

	
		void					setup						();
		void					update						();
		void					show						(bool is);
		void					enableDrawCallback			(bool is);
		void					createControlsCustom		();
		void					createControlsCustomFinalize();
		void					updateListDevices			();
		void					selectDevice				(string id);
		void					selectDeviceWithIndex		(int index);
		void					updateDeviceAnimationTitle	();
		void					updateDeviceUI				(Device*);
		void					updateDeviceColorUI			();
		void					drawUI						();
		void					handleEvents				(ofxUIEventArgs& e);
		bool					isHit						(int x, int y);
		bool					keyPressed					(int key);
		void					enableWindowCallbacks		();
		void					disableWindowCallbacks		();

		virtual string			getMidiSettingsPath			(){return "Gui/tools/midi/Devices.xml";}
		void					loadMidiSettings			();
		void					readMidiSettingsExtraBegin	(int which,string propName);
		void 					newMidiMessage				(ofxMidiMessage& eventArgs);
		void					handleMidiMessages			();

		void					onDevicePropChanged			(const void* sender, float& value);
 

	 	classProperties			m_properties;

        DeviceManager*      	mp_deviceManager;
		vector<string>			m_speakersId;

        ofxUIDropDownList*		mp_ddlDevices;
		ofxUICanvas*			mp_canvasDevice;
		ofxUILabel*				mp_lblDeviceTitle;
		ofxUIToggle*			mp_toggleDeviceUseRawVol;
		ofxUIToggle*			mp_toggleDeviceMute;
	    ofxUISlider*			mp_sliderDeviceVolMax;
	    ofxUITextInput*			mp_teDeviceVolMaxMax;
    	ofxUISlider* 			mp_sliderDeviceVolHistorySize;
    	ofxUIToggle* 			mp_toggleDeviceEnableStandby;
    	ofxUISlider* 			mp_sliderDeviceVolHistoryTh;
    	ofxUISlider* 			mp_sliderDeviceTimeStandby;
    	ofxUISlider* 			mp_sliderDeviceSampleVolStandby;

		ofxUIToggle*			mp_toggleColorEnable;
		ofxUISlider*			mp_sliderColorManualHue;
		ofxUISlider*			mp_sliderColorManualSaturation;
		ofxUISpacer*			mp_spacerColorManualHsb;
 
		ofxUIToggle*			mp_togglePacketsInvert;
		ofxUIToggle*			mp_togglePacketsReverse;
 
		ofxUISlider*			mp_sliderDeviceVolHistoryPingTh;
 

    	ofxUIToggle* 			mp_toggleDeviceEnableStandup;
		ofxUISlider*			mp_sliderStandupVol;
 
		ofxUIMovingGraphThreshold* mp_graphSoundValues;

		ofxUICanvas*			mp_canvasColorMode;
	
};
