//
//  toolALB.h
//  murmur
//
//  Created by Julien on 23/01/2015.
//
//

#pragma once
#include "tool.h"
#include "ofxRecorder.h"
#include "ofxVideoRecorder.h"


class toolALB : public tool
{
	public:
		toolALB					(toolManager*);

		void					createControlsCustom	();
		void					setup					();
		void					update					();
		void					handleEvents			(ofxUIEventArgs& e);
		void					startRecording			();
		void					endRecording			();
		string					getData					();

		ofxRecorder				m_recorder;
		ofxUIButton*			mp_btnRecord;
		bool					m_bRecording;
 
		ofxVideoRecorder		m_videoRecorder;
		ofImage					m_imageOffscreen;
};