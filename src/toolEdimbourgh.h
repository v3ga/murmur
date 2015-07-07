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
#include "midiInterface.h"
#include "classProperty.h"

class toolEdimbourgh : public tool, public midiInterface
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

		virtual string			getMidiSettingsPath		(){return "Gui/tools/midi/_Edimbourgh_.xml";}
		void					loadMidiSettings		(){midiInterface::loadMidiSettings(m_properties);}
		void					onPlayPause				(bool& value);
		void					onStop					(bool& value);


	 	classProperties			m_properties;
		ofxTimeline				m_timeline;
 


		ofxUIToggle*			mp_tgLoopTimeline;
		ofxUIToggle*			mp_tgAutoplayTimeline;
		ofxUIButton*			mp_btnSaveTimeline;
		ofxUIButton*			mp_btnPlayPauseTimeline;
		ofxUIButton*			mp_btnStopTimeline;

		string					m_timelineCurrentFolder;
		bool					m_bAutoplay;
 };