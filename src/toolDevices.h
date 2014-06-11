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

class toolDevices : public tool
{
	public:

		toolDevices				(toolManager* parent, DeviceManager*);

	
		void					setup						();
		void					show						(bool is);
		void					enableDrawCallback			(bool is);
		void					createControlsCustom		();
		void					createControlsCustomFinalize();
		void					updateListDevices			();
		void					selectDevice				(string id);
		void					selectDeviceWithIndex		(int index);
		void					updateDeviceAnimationTitle	();
		void					updateDeviceUI				(Device*);
		void					draw						();
		void					handleEvents				(ofxUIEventArgs& e);
		bool					isHit						(int x, int y);

        DeviceManager*      	mp_deviceManager;

        ofxUIDropDownList*		mp_ddlDevices;

		ofxUICanvas*			mp_canvasDevice;
		ofxUILabel*				mp_lblDeviceTitle;
	    ofxUISlider*			mp_sliderDeviceVolMax;
    	ofxUISlider* 			mp_sliderDeviceVolHistorySize;
    	ofxUIToggle* 			mp_toggleDeviceEnableStandby;
    	ofxUISlider* 			mp_sliderDeviceVolHistoryTh;
    	ofxUISlider* 			mp_sliderDeviceTimeStandby;
    	ofxUISlider* 			mp_sliderDeviceSampleVolStandby;
};
