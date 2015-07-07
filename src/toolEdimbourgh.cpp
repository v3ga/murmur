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
	mp_btnSaveTimeline 		= 0;
	mp_tgLoopTimeline 		= 0;
	mp_tgAutoplayTimeline	= 0;
	mp_btnPlayPauseTimeline	= 0;
	mp_btnStopTimeline		= 0;

	m_timelineCurrentFolder = "Gui/tools/_Edimbourgh_timeline/config01/";
	m_bAutoplay 			= false;
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

	    mp_canvas->addWidgetDown(new ofxUILabel("Timeline", OFX_UI_FONT_MEDIUM));
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1));

		mp_btnPlayPauseTimeline = new ofxUILabelButton("Play", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetDown( mp_btnPlayPauseTimeline );

		mp_btnStopTimeline = new ofxUILabelButton("Stop", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetRight( mp_btnStopTimeline );

		mp_tgAutoplayTimeline = new ofxUIToggle("Loop", false, dim, dim);
		mp_canvas->addWidgetRight( mp_tgAutoplayTimeline );

		mp_tgLoopTimeline = new ofxUIToggle("Autoplay", false, dim, dim);
		mp_canvas->addWidgetRight( mp_tgLoopTimeline );


		mp_btnSaveTimeline = new ofxUILabelButton("Save", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetDown( mp_btnSaveTimeline );

	    mp_canvas->addWidgetDown(new ofxUILabel("Timeline/setAnim", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1) );
		mp_canvas->addWidgetDown(new ofxUISlider("transition duration (s)", 0.1f, 1.0f, 0.15f, widthDefault, dim ));

		mp_canvas->autoSizeToFitWidgets();
		
		classProperty_bool* pPropPlayPause = new classProperty_bool("bPlayPause",0,classProperty_bool::MODE_BUTTON);
		m_properties.add( pPropPlayPause );
		ofAddListener(pPropPlayPause->onValueChanged, this, &toolEdimbourgh::onPlayPause);

		classProperty_bool* pPropStop = new classProperty_bool("bStop",0,classProperty_bool::MODE_BUTTON);
		m_properties.add( pPropStop );
		ofAddListener(pPropStop->onValueChanged, this, &toolEdimbourgh::onStop);

	}

	// Timeline
	createTimeline();
}

//--------------------------------------------------------------
void toolEdimbourgh::setup()
{
	OFAPPLOG->begin("toolEdimbourgh::setup()");
	OFAPPLOG->begin(" - autoplay = " + ofToString(m_bAutoplay));

	if (m_bAutoplay)
	{
		m_timeline.play();
		updateLayout();
	}
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void toolEdimbourgh::createTimeline()
{
	// Reset
	ofRemoveListener(m_timeline.events().bangFired, this, &toolEdimbourgh::bangFired);
	m_timeline.reset();

	// Create
	m_timeline.setSpacebarTogglePlay(false);
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
	handleMidiMessages();

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
}

//--------------------------------------------------------------
void toolEdimbourgh::updateLayout()
{
	if (mp_btnPlayPauseTimeline)
		mp_btnPlayPauseTimeline->getLabelWidget()->setLabel( m_timeline.getIsPlaying() ? "Pause" : "Play" );
}

//--------------------------------------------------------------
void toolEdimbourgh::drawUI()
{
	tool::drawUI();
	float margin = 10;
	m_timeline.setWidth(ofGetWidth()-2*margin);
	m_timeline.setOffset(ofVec2f(margin,ofGetHeight()-m_timeline.getHeight()-margin));
	m_timeline.draw();
}


//--------------------------------------------------------------
void toolEdimbourgh::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	if (name == "Save")
	{
		m_timeline.saveTracksToFolder(m_timelineCurrentFolder);
		m_timeline.save();
	}
	else if (name == "Loop")
	{
		m_timeline.setLoopType(  e.getToggle()->getValue() ?  OF_LOOP_NORMAL : OF_LOOP_NONE );
	}
	else if (name == "Play" || name == "Pause")
	{
		ofxUIButton* pBtn = e.getButton();
		if (pBtn->getValue()){
			m_timeline.togglePlay();
			updateLayout();
		}
	}
	else if (name == "Stop")
	{
		m_timeline.stop();
		m_timeline.setCurrentTimeSeconds(0);
		updateLayout();
	}
	else if (name == "Autoplay")
	{
		m_bAutoplay = e.getToggle()->getValue();
	}
	else if (name == "transition duration (s)")
	{
		AnimationManager& animManager = GLOBALS->getSurfaceMain()->getAnimationManager();
		animManager.M_setTransitionDuration( e.getSlider()->getScaledValue() );
	}
}

//--------------------------------------------------------------
void toolEdimbourgh::bangFired(ofxTLBangEventArgs& args)
{
	string trackName = args.track->getName();
	if (trackName == "setAnim")
	{
		AnimationManager& animManager = GLOBALS->getSurfaceMain()->getAnimationManager();

		string animationNameWithArgs = args.flag;
		vector<string> animParts = ofSplitString(animationNameWithArgs, "/");

		if (animParts.size() == 1)
		{
			animManager.M_setAnimation( animParts[0] );
		}
		else{
			if (animParts[1] == "d")
				animManager.M_setAnimationDirect( animParts[0] );
			else
				animManager.M_setAnimation( animParts[0] );
		}
	}
}

//--------------------------------------------------------------
void toolEdimbourgh::onPlayPause(bool& value)
{
	m_timeline.togglePlay();
	updateLayout();
}

//--------------------------------------------------------------
void toolEdimbourgh::onStop(bool& value)
{
	m_timeline.stop();
	m_timeline.setCurrentTimeSeconds(0);
	updateLayout();
}





