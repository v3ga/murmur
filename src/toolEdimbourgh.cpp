//
//  toolEdimbourgh.cpp
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#include "toolEdimbourgh.h"
#include "ofAppLog.h"
#include "globals.h"
#include "surface.h"
#include "animations.h"

//--------------------------------------------------------------
toolEdimbourgh::toolEdimbourgh(toolManager* pParent) : tool("_Edimbourgh_", pParent)
{
	m_timelineCurrentFolder = "Gui/tools/_Edimbourgh_timeline/config01/";
	mp_btnSaveTimeline = 0;
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

		mp_btnSaveTimeline = new ofxUILabelButton("Save", false, 100,dim);
		mp_canvas->addWidgetDown( mp_btnSaveTimeline );

		mp_canvas->autoSizeToFitWidgets();
	}


	// Timeline
	createTimeline();
}

//--------------------------------------------------------------
void toolEdimbourgh::setup()
{
	OFAPPLOG->begin("toolEdimbourgh::setup()");
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void toolEdimbourgh::createTimeline()
{
	// Reset
	ofRemoveListener(m_timeline.events().bangFired, this, &toolEdimbourgh::bangFired);
	m_timeline.reset();

	// Create
	m_timeline.setup();
	m_timeline.setWorkingFolder(m_timelineCurrentFolder);
	m_timeline.setAutosave(false);
	m_timeline.addFlags("setAnim");
	m_timeline.setDurationInSeconds(60);
	m_timeline.loadTracksFromFolder(m_timelineCurrentFolder);

	ofAddListener(m_timeline.events().bangFired, this, &toolEdimbourgh::bangFired);
}

//--------------------------------------------------------------
void toolEdimbourgh::update()
{
	DeviceManager* pDeviceManager = GLOBALS->mp_deviceManager;
	if (pDeviceManager)
	{
		int nbDevices = pDeviceManager->m_listDevices.size();
		for (int i=0;i<nbDevices;i++)
		{
			string ofxTLTrackName = pDeviceManager->m_listDevices[i]->m_id+".color";
			ofxTLTrack* pDeviceColorTrack = m_timeline.getTrack( ofxTLTrackName );
			

			if (pDeviceColorTrack == 0)
			{
				pDeviceColorTrack = m_timeline.addColors(ofxTLTrackName);
			m_timeline.loadTracksFromFolder(m_timelineCurrentFolder);

			 }
			
			if (pDeviceColorTrack)
			{
				ofColor colorDevice = ((ofxTLColorTrack*)pDeviceColorTrack)->getColor();
				pDeviceManager->m_listDevices[i]->setColorHueSaturation( colorDevice.getHue(), colorDevice.getSaturation());
			}
		}
	
	}

//	if (m_timeline.is)
	

	
}

//--------------------------------------------------------------
void toolEdimbourgh::drawUI()
{
	tool::drawUI();
//	ofPushMatrix();
//	ofTranslate(0,ofGetHeight()-m_timeline.getHeight());
	float margin = 10;
	m_timeline.setWidth(ofGetWidth()-2*margin);
	m_timeline.setOffset(ofVec2f(margin,ofGetHeight()-m_timeline.getHeight()-margin));
	m_timeline.draw();
//	ofPopMatrix();
}


//--------------------------------------------------------------
void toolEdimbourgh::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	if (name == "Save")
	{
		ofLog() << "Save";
				m_timeline.saveTracksToFolder(m_timelineCurrentFolder);
m_timeline.save();
	}
}

//--------------------------------------------------------------
void toolEdimbourgh::bangFired(ofxTLBangEventArgs& args)
{
	string trackName = args.track->getName();
	if (trackName == "setAnim")
	{
		string animationName = args.flag;
		AnimationManager& animManager = GLOBALS->getSurfaceMain()->getAnimationManager();
		animManager.M_setAnimation( animationName );
	}
}





