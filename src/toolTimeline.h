//
//  toolTimeline.h
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

class toolTimeline : public tool, public midiInterface
{
	public:
		toolTimeline			(toolManager*);

		void					createControlsCustom	();

		void					setup					();
		void					loadData				();
		void					update					();
		void					updateLayout			();
		void					drawUI					();
		void					handleEvents			(ofxUIEventArgs& e);
		void					bangFired				(ofxTLBangEventArgs& args);
		bool					keyPressed				(int key){return true;}

		void					createTimeline			();

		virtual string			getMidiSettingsPath		(){return "Gui/tools/midi/Timeline.xml";}
		void					loadMidiSettings		(){midiInterface::setClassProperties(&m_properties) ; midiInterface::loadMidiSettings();}
		void					onPlayPause				(bool& value);
		void					onStop					(bool& value);
 

	 	classProperties			m_properties;
		ofxTimeline				m_timeline;
 


		ofxUIToggle*			mp_tgLoopTimeline;
		ofxUIToggle*			mp_tgAutoplayTimeline;
		ofxUIButton*			mp_btnSaveTimeline;
		ofxUIButton*			mp_btnPlayPauseTimeline;
		ofxUIButton*			mp_btnStopTimeline;
		ofxUITextInput*			mp_teDurationTimeline;
		ofxUIToggle*			mp_tgPlayWithTimecode;
 

		ofxUITextInput*			mp_teNameNewTimeline;
		ofxUIButton*			mp_btnNewTimeline;
		ofxUIDropDownList*		mp_ddConfigTimeline;
		ofxUIButton*			mp_btnLoadTimeline;

		string					m_timelineCurrentFolder;
		bool					m_bAutoplay;
 
		bool					m_bPlayWithTimecode;		// toolMidi
 
 	protected:
		void					createNewTimeline			();
		vector<string>			getTimelineConfigNamesList	();
		string					getConfigPath				(string configName){return sm_timelineDir+configName+"/";}
 
		string					m_timelineConfigName;
		static string			sm_timelineDir;
};