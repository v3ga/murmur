//
//  toolMidi.cpp
//  murmur
//
//  Created by Julien on 10/09/2015.
//
//

#include "toolMidi.h"
#include "ofxMidi.h"
#include "midiInterface.h"

toolMidi::toolMidi(toolManager* parent) : tool("Midi", parent)
{
	mp_midiIns		= 0;
	mp_ddMidiPorts 	= 0;
}

toolMidi::~toolMidi()
{
	m_midiInterfaces.clear();
}

//--------------------------------------------------------------
void toolMidi::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float dim = 16;
		int widthDefault = 440;

	    mp_canvas->addWidgetDown( new ofxUILabel("Midi",OFX_UI_FONT_LARGE) );
    	mp_canvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );

		if (mp_midiIns && mp_midiIns->size()>0)
		{
			int nbMidiIns = mp_midiIns->size();
			vector<string> midiPorts;
			for (int i=0; i<nbMidiIns ; i++)
			{
				int port = (*mp_midiIns)[i]->getPort();
				string portName = ofxMidiIn::getPortName( port );
				if (portName != "")
				{
					string strPort = ofToString( port ) + " - " + portName;
					midiPorts.push_back( strPort );
				}
			}
			
			if (midiPorts.size()>0)
			{
				mp_ddMidiPorts = new ofxUIDropDownList("ports", midiPorts);
				mp_ddMidiPorts->setAutoClose(true);
				mp_canvas->addWidgetDown( mp_ddMidiPorts );
			}
		}

		mp_canvas->autoSizeToFitWidgets();

		if (mp_midiIns && mp_midiIns->size()>0)
		{
			ofxUIRectangle* pCanvasRect = mp_canvas->getRect();
		
			int nbMidiInterfaces = m_midiInterfaces.size();
			float y = pCanvasRect->getY();
			for (int i=0; i<nbMidiInterfaces; i++)
			{
            	ofxUICanvas* pMidiCanvas = new ofxUICanvas(240, y, widthDefault+50, 100);
				midiInterface* pMidiInterface = m_midiInterfaces[i];

		    	pMidiCanvas->addWidgetDown( new ofxUILabel(pMidiInterface->m_name,OFX_UI_FONT_MEDIUM) );
				pMidiCanvas->addWidgetDown( new ofxUISpacer(widthDefault-20, 1) );

				bool hasProperties = false;
				for (int i=0; i<mp_midiIns->size(); i++)
				{
					midiPort* pMidiPort = pMidiInterface->getMidiPort(i);
					if (pMidiPort)
					{
						map<int,classProperty*>& mapProp = pMidiPort->m_mapMidiControlToProp;
						map<int,classProperty*>::iterator it = mapProp.begin();
						if(mapProp.size()>0)
						{
							hasProperties = true;
							for ( ; it!=mapProp.end(); ++it)
							{
						    	pMidiCanvas->addWidgetDown( new ofxUILabel(widthDefault/2, it->second->m_name,OFX_UI_FONT_SMALL) );

								ofxUITextInput* pTePort = new ofxUITextInput("te_"+it->second->m_name, ofToString( 0 ), 24, dim, widthDefault/2);
								ofxUITextInput* pTeControl = new ofxUITextInput("te_"+it->second->m_name, ofToString( it->first ), 24, dim, widthDefault/2);
						    	pMidiCanvas->addWidgetRight( pTePort );
						    	pMidiCanvas->addWidgetRight( pTeControl );
							}
						}
					}
				}

					if (!hasProperties)
					  {
							   pMidiCanvas->addWidgetDown( new ofxUILabel("No midi properties defined.",OFX_UI_FONT_SMALL) );
						}

				pMidiCanvas->autoSizeToFitWidgets();

				y+=pMidiCanvas->getRect()->getHeight();

				m_mapMidiCanvas[pMidiInterface] = pMidiCanvas;
				//mp_canvas->addWidgetDown( pMidiCanvas );
			}
		 }
		
		

		ofAddListener(mp_canvas->newGUIEvent, this, &toolMidi::handleEvents);
	}
}


//--------------------------------------------------------------
void toolMidi::setMidiIns(vector<ofxMidiIn*>* pMidiIns)
{
	mp_midiIns = pMidiIns;
}

//--------------------------------------------------------------
void toolMidi::registerMidiInterface(midiInterface* pMidiInterface)
{
	m_midiInterfaces.push_back(pMidiInterface);
}

//--------------------------------------------------------------
void toolMidi::setup()
{

}


//--------------------------------------------------------------
void toolMidi::update()
{
}

//--------------------------------------------------------------
void toolMidi::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
    if (name == "ports")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        //vector<ofxUIWidget *> &selected = ddlist->getSelected();
		vector<int>& selectedIndices = ddlist->getSelectedIndeces();
        if (selectedIndices.size()==1)
		{
			// ofLog() << selectedIndices[0];
			// Get sound current
			// string soundName = selected[0]->getName();
		}
    }
	
}


