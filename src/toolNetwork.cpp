//
//  toolNetwork.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolNetwork.h"
#include "globals.h"


toolNetwork* toolNetwork::smp_this = 0;

//--------------------------------------------------------------
toolNetwork::toolNetwork(toolManager* parent) : tool("Network", parent)
{
    m_oscReceiverIP 	= "127.0.0.1";
    m_oscReceiverPort 	= 1234;
	m_isLogMessagesIn	= true;
	m_isLogMessagesOut	= false;
	m_logMessagesOSCMax = 20;
	m_isLogEnable		= false;
	
	mp_consoleOsc		= 0;
	
	smp_this			= this;
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
    	mp_canvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );

	    mp_canvas->addWidgetDown(new ofxUILabel("port : ", fontType));
    	mp_canvas->addWidgetRight(new ofxUITextInput("Port", ofToString(m_oscReceiverPort) , 100, dim,0,0,fontType));

		mp_canvas->addWidgetDown(new ofxUILabel("Console OSC", OFX_UI_FONT_MEDIUM));
		mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 2));
		mp_consoleOsc = new ofxUITextArea("Console", "", widthDefault, 340,0,0,fontType);

		mp_canvas->addWidgetDown(new ofxUIToggle("Enable log messages", &m_isLogEnable, dim, dim) );
		mp_canvas->addWidgetDown(new ofxUIToggle("In", &m_isLogMessagesIn, dim, dim) );
		mp_canvas->addWidgetRight(new ofxUIToggle("Out", &m_isLogMessagesOut, dim, dim) );
		mp_canvas->addWidgetDown(mp_consoleOsc);


		mp_canvas->autoSizeToFitWidgets();
		mp_canvas->disableAppDrawCallback();
	}
}


//--------------------------------------------------------------
void toolNetwork::logMessageOSC(const ofxOscMessage& message, bool isIn)
{
	if (smp_this)
		smp_this->logMessage(message, isIn);
}

//--------------------------------------------------------------
void toolNetwork::logMessage(const ofxOscMessage& message, bool isIn)
{
	if (m_isLogEnable == false) return;

	if (isIn && m_isLogMessagesIn)
	{
		m_logMessagesOSC.insert( m_logMessagesOSC.begin(), "[in:"+message.getRemoteIp()+"] "+message.getAddress() );
	}

	if (!isIn && m_isLogMessagesOut)
	{
		m_logMessagesOSC.insert( m_logMessagesOSC.begin(), "[out] "+message.getAddress() );
	}
	
	if (m_logMessagesOSC.size() >= m_logMessagesOSCMax)
		m_logMessagesOSC.pop_back();

	if (mp_consoleOsc)
	{
		m_logConsole = "";
		for (int i=0; i<m_logMessagesOSC.size(); i++)
		{
			m_logConsole+=m_logMessagesOSC[i]+"\n\n";
		}
			
			
		mp_consoleOsc->setTextString(m_logConsole);
	}
}

/*
   if (pThis && pThis->mp_consoleJs)
   {
		pThis->m_listLogJs.insert(pThis->m_listLogJs.begin(), message);
		if (pThis->m_listLogJs.size() > pThis->m_listLogJsMax){
			pThis->m_listLogJs.pop_back();
		}
		string s = "";
		for (int i=0;i<pThis->m_listLogJs.size();i++){
			s = s+pThis->m_listLogJs[i]+"\n";
		}
		pThis->mp_consoleJs->setTextString(s);
   }

*/


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
