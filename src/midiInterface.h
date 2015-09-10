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

class midiPort
{
	public:
	
		midiPort					(int port)
		{
			m_port = port;
		}
 
		void addPropertyForControl(int control, classProperty* pProp)
		{
			m_mapMidiControlToProp[control] = pProp;
		}
	
		int							m_port;
		 map<int,classProperty*>	m_mapMidiControlToProp;
};

class midiInterface
{
	public:
	
		typedef map<int,classProperty*> TMapMidiToProp;
	
		string					m_name;
		ofxXmlSettings			m_midiSettings;

				void			setMidiName					(string s){m_name = s;}
		virtual string			getMidiSettingsPath			(){return "";}
		virtual	void			loadMidiSettings			(classProperties&);
		virtual	void			readMidiSettingsExtraBegin	(int which, string propName); // <midi> node index
		virtual	void			readMidiSettingsExtraEnd	(int which, string propName); // <midi> node index


		virtual	void			handleMidiMessages			();
		virtual void 			newMidiMessage				(ofxMidiMessage& eventArgs);
 
 		classProperty*			getPropertyForPortAndControl(int port, int control);
 
 		vector<midiPort*>		m_midiPorts;
		midiPort*				getMidiPort					(int which);

//		TMapMidiToProp			m_mapMidiToProp;
//		map<int, TMapMidiToProp*>m_mapPortToProp;
 		vector<ofxMidiMessage>	m_midiMessagesToHandle;
		ofMutex					m_midiMutex;
};