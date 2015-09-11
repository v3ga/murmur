//
//  toolMidi.h
//  murmur
//
//  Created by Julien on 10/09/2015.
//
//

#include "tool.h"

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
		void					setup					();
		void					update					();
		void					createControlsCustom	();
		void					handleEvents			(ofxUIEventArgs& e);
		void					handleEventsMidiInterface(ofxUIEventArgs& e);


		void					hideMidiInterfaces		();
		midiInterface*			getMidiInterfaceByName	(string name);


		vector<ofxMidiIn*>*		mp_midiIns;
		vector<midiInterface*>	m_midiInterfaces;
		midiInterface*			mp_midiInterfaceCurrent;
 
		// Controls
		ofxUIDropDownList*		mp_ddMidiInterfaces;
		ofxUICanvas*			mp_canvasMidiInterfaceCurrent;

 
 
 		map<midiInterface*, ofxUICanvas*>	m_mapMidiCanvas;
 
};