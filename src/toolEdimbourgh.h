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
		void					drawUI					();
		void					handleEvents			(ofxUIEventArgs& e);
		void					bangFired				(ofxTLBangEventArgs& args);
		bool					keyPressed				(int key){return true;}

		void					createTimeline			();

	 	classProperties			m_properties;
		ofxTimeline				m_timeline;
 
		ofxUIButton*			mp_btnSaveTimeline;
		string					m_timelineCurrentFolder;
 
 };