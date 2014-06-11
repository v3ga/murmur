//
//  toolVideo.h
//  murmur
//
//  Created by Julien on 06/06/2014.
//
//

#pragma once

#include "tool.h"
#include "ofxVideoRecorder.h"

class toolVideo : public tool
{
	public:
		toolVideo				(toolManager*);

		void					setup		();
		void					update		();

		ofImage 				recordingImage;
	    ofxVideoRecorder    	vidRecorder;
};