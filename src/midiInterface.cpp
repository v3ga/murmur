//
//  midiInterface.cpp
//  murmur
//
//  Created by Julien on 07/07/2015.
//
//

#include "midiInterface.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
void midiInterface::loadMidiSettings(classProperties& properties)
{
	OFAPPLOG->begin("midiInterface::loadMidiSettings()");
	string file = getMidiSettingsPath();
	OFAPPLOG->println(" - file="+file);

	ofFile f(file);
	if (f.exists())
	{
		if (m_midiSettings.load(file))
		{
			OFAPPLOG->println(" - ok loaded settings.");
			int nb = m_midiSettings.getNumTags("midi");
			for (int i=0;i<nb;i++)
			{
				int 	control 	= m_midiSettings.getAttribute("midi", "control", 0, i);
				if (control>0)
				{
					string 	propName 	= m_midiSettings.getAttribute("midi", "property", "???", i);
					readMidiSettingsExtraBegin(i,propName);
					
					classProperty* pProp = properties.get(propName);
					if(pProp)
					{
						m_mapMidiToProp[control] = 	pProp;
						OFAPPLOG->println(" - defining control "+ofToString(control) + " for property '"+pProp->m_name+"'");

					}
					readMidiSettingsExtraEnd(i,propName);
				}
			}
		}
		else
		{
			OFAPPLOG->println(" - error loading settings.");
		}
	}
	else
	{
	 OFAPPLOG->println(" - warning, midi file not found.");
	}
	OFAPPLOG->end();

}

//--------------------------------------------------------------
void midiInterface::readMidiSettingsExtraBegin(int which, string propName)
{
}

void midiInterface::readMidiSettingsExtraEnd(int which, string propName)
{
}


//--------------------------------------------------------------
void midiInterface::handleMidiMessages()
{
	m_midiMutex.lock();
	int nbMidiMessages = m_midiMessagesToHandle.size();
	for (int i=0;i<nbMidiMessages;i++)
	{
		ofxMidiMessage& midiMessage = m_midiMessagesToHandle[i];

		int control = midiMessage.control;

		if ( m_mapMidiToProp.find( control ) != m_mapMidiToProp.end() )
		{
			classProperty* pProp = m_mapMidiToProp[control];
			pProp->setValueFromMidiMessage( midiMessage );
		}
	}
	m_midiMessagesToHandle.clear();
	m_midiMutex.unlock();
	
}

//--------------------------------------------------------------
void midiInterface::newMidiMessage(ofxMidiMessage& midiMessage)
{
	m_midiMutex.lock();
	m_midiMessagesToHandle.push_back( ofxMidiMessage(midiMessage) ); // make a copy
	m_midiMutex.unlock();
}


