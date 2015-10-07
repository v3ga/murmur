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
#include "ofAppLog.h"

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
 
 
		classProperty* deleteProperty(string name)
		{
			OFAPPLOG->begin("midiPort::deleteProperty('"+name+"')");
			classProperty* pClassProp = 0;
			map<int,classProperty*>::iterator it = m_mapMidiControlToProp.begin();
			for ( ; it != m_mapMidiControlToProp.end() ; ++it)
			{
				if ( it->second->m_name == name)
				{
					OFAPPLOG->println("- found it, deleting it.");
					m_mapMidiControlToProp.erase(it);
					OFAPPLOG->end();
					return it->second;
				}
			}
			OFAPPLOG->end();
			return 0;
		}
 
		classProperty* deletePropertyForControl(int control)
		{
			OFAPPLOG->begin("midiPort::deletePropertyForControl('"+ofToString(control)+"')");
			classProperty* pClassProp = 0;
			map<int,classProperty*>::iterator it = m_mapMidiControlToProp.find(control);
			if (it != m_mapMidiControlToProp.end())
			{
				pClassProp = m_mapMidiControlToProp[control];
				m_mapMidiControlToProp.erase(it);
				if (pClassProp)
					OFAPPLOG->println(" - property is "+pClassProp->m_name);
			}
			OFAPPLOG->end();
			return pClassProp;
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
		virtual string			getMidiSettingsPath			(string suffix=""){return "";}
		virtual void			saveMidiSettings			();
		virtual	void			loadMidiSettings			();
		virtual	void			readMidiSettingsExtraBegin	(int which, string propName); // <midi> node index
		virtual	void			readMidiSettingsExtraEnd	(int which, string propName); // <midi> node index
		virtual	void			exportMidiSettingsTemplate	();

		virtual	void			handleMidiMessages			();
		virtual void 			newMidiMessage				(ofxMidiMessage& eventArgs);
		virtual	void			onPropertyMidiModified		(classProperty* pProperty){}
 

		bool					isPortValid					(int port);
 		classProperty*			getPropertyForPortAndControl(int port, int control);

		int						getPortForProperty			(string propertyName);
		void					setPortForProperty			(int port, string propertyName);
 
		int						getControlForProperty		(string propertyName);
		void					setControlForProperty		(int control, string propertyName);
 

		virtual	void			deleteMidiPorts				();
 
 		vector<midiPort*>		m_midiPorts;
		midiPort*				getMidiPort					(int which);

 		vector<ofxMidiMessage>	m_midiMessagesToHandle;
		ofMutex					m_midiMutex;
};