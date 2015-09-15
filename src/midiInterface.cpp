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
midiInterface::midiInterface()
{
	mp_classProperties = 0;
}

//--------------------------------------------------------------
void midiInterface::deleteMidiPorts()
{
	vector<midiPort*>::iterator it = m_midiPorts.begin();
	for ( ; it != m_midiPorts.end(); ++it)
	{
		delete *it;
	}
	m_midiPorts.clear();
}


//--------------------------------------------------------------
void midiInterface::loadMidiSettings()
{
	if (mp_classProperties==0) return;

	deleteMidiPorts();
	

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
					
					classProperty* pProp = mp_classProperties->get(propName);
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
void midiInterface::saveMidiSettings()
{
	ofxXmlSettings midiSettings;
	
	vector<midiPort*>::iterator it = m_midiPorts.begin();
	int index = 0;
	for ( ; it != m_midiPorts.end(); ++it)
	{
		int midiPort = (*it)->m_port;
		
		map<int,classProperty*>::iterator it2 = (*it)->m_mapMidiControlToProp.begin();
		for ( ; it2!=(*it)->m_mapMidiControlToProp.end(); ++it2)
		{
			int midiControl = it2->first;
			classProperty* pProperty = it2->second;
		
			if (pProperty)
			{
				midiSettings.addTag("midi");
				midiSettings.setAttribute("midi", "port", 		midiPort, index);
				midiSettings.setAttribute("midi", "control", 	midiControl, index);
				midiSettings.setAttribute("midi", "property", 	pProperty->m_name, index);

				index++;
			}
		}

	}
	
	midiSettings.save( getMidiSettingsPath() );
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
		classProperty* pProp = getPropertyForPortAndControl(midiMessage.portNum, midiMessage.control);
		if (pProp)
			pProp->setValueFromMidiMessage( midiMessage );
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
bool midiInterface::isPortValid(int port)
{
	vector<midiPort*>::iterator it = m_midiPorts.begin();
	for ( ; it != m_midiPorts.end(); ++it)
	{
		if ((*it)->m_port == port)
			return true;
	}
	return false;
}


//--------------------------------------------------------------
classProperty* midiInterface::getPropertyForPortAndControl(int port, int control)
{
	midiPort* pMidiPort = getMidiPort(port);
	if (pMidiPort)
		return pMidiPort->m_mapMidiControlToProp[control];
	return 0;
}

//--------------------------------------------------------------
int midiInterface::getPortForProperty(string propertyName)
{
	vector<midiPort*>::iterator it = m_midiPorts.begin();
	for ( ; it != m_midiPorts.end(); ++it)
	{
		map<int,classProperty*>::iterator it2 = (*it)->m_mapMidiControlToProp.begin();
		for ( ; it2 != (*it)->m_mapMidiControlToProp.end(); ++it2)
		{
			if (it2->second->m_name == propertyName)
				return (*it)->m_port;
		}
	}

	return -1;
}


//--------------------------------------------------------------
void midiInterface::setPortForProperty(int port, string propertyName)
{
	int currentPort = getPortForProperty(propertyName);
	if (currentPort>-1 && currentPort != port && isPortValid(port))
	{
		midiPort* pMidiPort = getMidiPort(currentPort);
		if (pMidiPort)
		{
		}
	}
	
	return currentPort;
}



//--------------------------------------------------------------
int midiInterface::getControlForProperty(string propertyName)
{
	vector<midiPort*>::iterator it = m_midiPorts.begin();
	for ( ; it != m_midiPorts.end(); ++it)
	{
		map<int,classProperty*>::iterator it2 = (*it)->m_mapMidiControlToProp.begin();
		for ( ; it2 != (*it)->m_mapMidiControlToProp.end(); ++it2)
		{
			if (it2->second->m_name == propertyName)
				return it2->first;
		}
	}
	return -1;
}

//--------------------------------------------------------------
void midiInterface::setControlForProperty(int control, string propertyName)
{
	int currentControl = getControlForProperty(propertyName);
	if (currentControl != control)
	{
		bool erased = false;
		vector<midiPort*>::iterator it = m_midiPorts.begin();
		for ( ; it != m_midiPorts.end(); ++it)
		{
			map<int,classProperty*>::iterator it2 = (*it)->m_mapMidiControlToProp.begin();
			classProperty* pProperty = 0;
			for ( ; it2 != (*it)->m_mapMidiControlToProp.end(); ++it2)
			{
				pProperty = it2->second;
				if (pProperty->m_name == propertyName)
				{
					(*it)->m_mapMidiControlToProp.erase(it2);
					erased = true;
					// (*it)->m_mapMidiControlToProp.ins
					break;
				}
			}
			if (erased && pProperty)
			{
				(*it)->addPropertyForControl(control, pProperty);
			}
			
		}
		
	}
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



