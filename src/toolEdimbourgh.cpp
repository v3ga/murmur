//
//  toolEdimbourgh.cpp
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#include "toolEdimbourgh.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
toolEdimbourgh::toolEdimbourgh(toolManager* pParent) : tool("_Edimbourgh_", pParent)
{
}

//--------------------------------------------------------------
void toolEdimbourgh::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float dim = 16;
		int widthDefault = 320;

		// Colonne 1
	    mp_canvas->addWidgetDown( new ofxUILabel("Edimbourgh",OFX_UI_FONT_LARGE) );
    	mp_canvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );

//		mp_btnRecord = new ofxUILabelButton("Record", false, 100,dim);
//		mp_canvas->addWidgetDown( mp_btnRecord );

		mp_canvas->autoSizeToFitWidgets();
	}
}

//--------------------------------------------------------------
void toolEdimbourgh::setup()
{
	OFAPPLOG->begin("toolEdimbourgh::setup()");
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void toolEdimbourgh::update()
{
}

//--------------------------------------------------------------
void toolEdimbourgh::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	if (name == "Record")
	{
	}
}






