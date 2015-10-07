//
//  toolMidi.h
//  murmur
//
//  Created by Julien on 10/09/2015.
//
//

#include "tool.h"
#include "ofxMTCReceiver.h"

class ofxMidiIn;
class midiInterface;

class toolMidi : public tool
{
	public:
		toolMidi				(toolManager*);
		~toolMidi				();

		void					setMidiIns				(vector<ofxMidiIn*>*);
		void					registerMidiInterface	(midiInterface*);

		void					show					(bool is);
		void					drawUI					();
		void					setup					();
		void					loadData				();
		void					saveData				();
		void					update					();
		void					updateLayout			();
		void					createControlsCustom	();
		void					handleEvents			(ofxUIEventArgs& e);
		void					handleEventsMidiInterface(ofxUIEventArgs& e);
		void					handleEventsTimecode	(ofxUIEventArgs& e);
		void					handleMTCMessage		(MTCEventArgs& e);
 


		void					hideMidiInterfaces		();
		midiInterface*			getMidiInterfaceByName	(string name);
 
		static int				sm_timecodeMillis;


	private:
		int						getPortTimecode			(string s){return ofToInt(s);}
		void					onPortTimecodeChanged	();
 
		string					getTimecodePathFile		();
 

		vector<ofxMidiIn*>*		mp_midiIns;
		vector<midiInterface*>	m_midiInterfaces;
		midiInterface*			mp_midiInterfaceCurrent;
 
		 ofxMTCReceiver			m_MTCReceiver;
		 int					m_portTimecode;
		 bool					m_bDriveTimeline;


 
		// Controls
		ofxUIDropDownList*		mp_ddMidiInterfaces;
		ofxUICanvas*			mp_canvasMidiInterfaceCurrent;
 
 		map<midiInterface*, ofxUICanvas*>	m_mapMidiCanvas;
		ofxUICanvas*			mp_canvasTimecode;
		ofxUILabel*				mp_lblTimecodeValue;
 

};