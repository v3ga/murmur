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
				int 	port		= m_midiSettings.getAttribute("midi", "port", 		0, i);
				int 	control 	= m_midiSettings.getAttribute("midi", "control", 	0, i);
//							OFAPPLOG->println(" - port="+ofToString(port)+",control="+ofToString(control));

				if (port >=0 && control>0)
				{
					string 	propName 	= m_midiSettings.getAttribute("midi", "property", "???", i);
					readMidiSettingsExtraBegin(i,propName);
					
					classProperty* pProp = properties.get(propName);
					if(pProp)
					{
						midiPort* pMidiPort = getMidiPort(port);
						if (pMidiPort == 0){
							pMidiPort = new midiPort(port);
							m_midiPorts.push_back( pMidiPort );
							OFAPPLOG->println(" - creating port["+ofToString(port)+"]");
						}
					
						if (pMidiPort)
						{
							pMidiPort->addPropertyForControl(control, pProp);
							OFAPPLOG->println(" - defining on port ["+ofToString(port)+"] control "+ofToString(control) + " for property '"+pProp->m_name+"' (size of port map="+(ofToString(pMidiPort->m_mapMidiControlToProp.size()))+")");
						}
					}
					else
						OFAPPLOG->println(" - property '"+propName+"' not found in interface");
		 

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

		int port = midiMessage.portNum;
		int control = midiMessage.control;
		ofLog() << port << "/" << control;

		classProperty* pProp = getPropertyForPortAndControl(port, control);

//		if ( m_mapMidiToProp.find( control ) != m_mapMidiToProp.end() )
		{
//			classProperty* pProp = m_mapMidiToProp[control];
			if (pProp){
				pProp->setValueFromMidiMessage( midiMessage );
				ofLog() << pProp->m_name;
					}
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

//--------------------------------------------------------------
classProperty* midiInterface::getPropertyForPortAndControl(int port, int control)
{
	midiPort* pMidiPort = getMidiPort(port);
	if (pMidiPort)
		return pMidiPort->m_mapMidiControlToProp[control];

/*	if ( m_mapPortToProp.find( port ) != m_mapPortToProp.end() )
	{
		ofLog() << ofToString( (*m_mapPortToProp[port]).size() ) << " / " << "pControl=" << ofToString( (int) (*m_mapPortToProp[port])[control] );
		return (*m_mapPortToProp[port])[control];
	}
*/
	return 0;
}

//--------------------------------------------------------------
midiPort* midiInterface::getMidiPort(int which)
{
	vector<midiPort*>::iterator it = m_midiPorts.begin();
	for ( ; it != m_midiPorts.end(); ++it){
		if ((*it)->m_port == which)
			return *it;
	}
	return 0;
}



