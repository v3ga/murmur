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

		void					setup					();
		void					update					();
		void					createControlsCustom	();
		void					handleEvents			(ofxUIEventArgs& e);

		vector<ofxMidiIn*>*		mp_midiIns;
		vector<midiInterface*>	m_midiInterfaces;
 
		// Controls
		ofxUIDropDownList*		mp_ddMidiPorts;
 
 
 		map<midiInterface*, ofxUICanvas*>	m_mapMidiCanvas;
 
};