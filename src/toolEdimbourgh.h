//
//  toolEdimbourgh.h
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#pragma once

#include "tool.h"
#include "classProperty.h"
#include "ofxTimeline.h"

class toolEdimbourgh : public tool
{
	public:
		toolEdimbourgh			(toolManager*);

		void					createControlsCustom	();

		void					setup					();
		void					update					();
		void					updateLayout			();
		void					drawUI					();
		void					handleEvents			(ofxUIEventArgs& e);
		void					bangFired				(ofxTLBangEventArgs& args);
		bool					keyPressed				(int key){return true;}

		void					createTimeline			();

	 	classProperties			m_properties;
		ofxTimeline				m_timeline;


		ofxUIToggle*			mp_tgLoopTimeline;
		ofxUIButton*			mp_btnSaveTimeline;
		ofxUIButton*			mp_btnPlayPauseTimeline;
		ofxUIButton*			mp_btnStopTimeline;
		string					m_timelineCurrentFolder;
 };