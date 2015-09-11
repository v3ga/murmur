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

//--------------------------------------------------------------
class midiPort
{
	public:
	
		midiPort(int port)
		{
			m_port = port;
		}
 
 		~midiPort()
		{
			m_mapMidiControlToProp.clear();
		}
 
		void addPropertyForControl(int control, classProperty* pProp)
		{
			m_mapMidiControlToProp[control] = pProp;
		}
	
		int							m_port;
		map<int,classProperty*>		m_mapMidiControlToProp;
};

//--------------------------------------------------------------
class midiInterface
{
	public:
		midiInterface			();

	
		string					m_name;
		ofxXmlSettings			m_midiSettings;
		classProperties*		mp_classProperties;

				void			setMidiName					(string s){m_name = s;}
				void			setClassProperties			(classProperties* p){mp_classProperties = p;}
		virtual string			getMidiSettingsPath			(){return "";}
		virtual void			saveMidiSettings			();
		virtual	void			loadMidiSettings			();
		virtual	void			readMidiSettingsExtraBegin	(int which, string propName); // <midi> node index
		virtual	void			readMidiSettingsExtraEnd	(int which, string propName); // <midi> node index


		virtual	void			handleMidiMessages			();
		virtual void 			newMidiMessage				(ofxMidiMessage& eventArgs);
 
 		classProperty*			getPropertyForPortAndControl(int port, int control);
		void					setPortForProperty			(int port, string propertyName);
		int						getControlForProperty		(string propertyName);
		void					setControlForProperty		(int control, string propertyName);
		virtual	void			deleteMidiPorts				();
 
 		vector<midiPort*>		m_midiPorts;
		midiPort*				getMidiPort					(int which);

 		vector<ofxMidiMessage>	m_midiMessagesToHandle;
		ofMutex					m_midiMutex;
};