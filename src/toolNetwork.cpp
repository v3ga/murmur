//
//  toolNetwork.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolNetwork.h"
#include "globals.h"

//--------------------------------------------------------------
toolNetwork::toolNetwork(toolManager* parent) : tool("Network", parent)
{
    m_oscReceiverIP 	= "127.0.0.1";
    m_oscReceiverPort 	= 1234;
}
	
//--------------------------------------------------------------
void toolNetwork::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float dim = 16;
		int widthDefault = 320;

	    mp_canvas->addWidgetDown( new ofxUILabel("Network",OFX_UI_FONT_LARGE) );
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 2));

	    mp_canvas->addWidgetDown(new ofxUILabel("port : ", fontType));
    	mp_canvas->addWidgetRight(new ofxUITextInput("Port", ofToString(m_oscReceiverPort) , 100, dim,0,0,fontType));
	}
}

//--------------------------------------------------------------
void toolNetwork::setup()
{
	// m_oscReceiver.setup(m_oscReceiverPort);
    //GLOBALS->mp_oscReceiver = &m_oscReceiver;
}

//--------------------------------------------------------------
void toolNetwork::update()
{
	// m_oscReceiver.update();
}

//--------------------------------------------------------------
void toolNetwork::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();

	if (name == "Port")
	{
        m_oscReceiverPort = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );
		//printf("m_oscReceiverPort=%d\n", m_oscReceiverPort);
	}

}
