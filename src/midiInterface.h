//
//  midiInterface.h
//  murmur
//
//  Created by Julien on 07/07/2015.
//
//

#pragma once
#include "ofxXmlSettings.h"
#include "ofxMidi.h"
#include "classProperty.h"

class midiInterface
{
	public:
	
		ofxXmlSettings			m_midiSettings;

		virtual string			getMidiSettingsPath			(){return "";}
		virtual	void			loadMidiSettings			(classProperties&);
		virtual	void			readMidiSettingsExtraBegin	(int which, string propName); // <midi> node index
		virtual	void			readMidiSettingsExtraEnd	(int which, string propName); // <midi> node index


		virtual	void			handleMidiMessages		();
		virtual void 			newMidiMessage			(ofxMidiMessage& eventArgs);

		map<int,classProperty*>	m_mapMidiToProp;
 		vector<ofxMidiMessage>	m_midiMessagesToHandle;
		ofMutex					m_midiMutex;
};