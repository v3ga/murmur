//
//  toolTimeline.cpp
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#include "toolTimeline.h"
#include "ofAppLog.h"
#include "globals.h"
#include "surface.h"
#include "animations.h"
#include "toolMidi.h"

string toolTimeline::sm_timelineDir = "Gui/tools/Timeline/";

//--------------------------------------------------------------
toolTimeline::toolTimeline(toolManager* pParent) : tool("Timeline", pParent)
{
	mp_btnSaveTimeline 		= 0;
	mp_tgLoopTimeline 		= 0;
	mp_tgAutoplayTimeline	= 0;
	mp_btnPlayPauseTimeline	= 0;
	mp_btnStopTimeline		= 0;
	mp_teDurationTimeline	= 0;
	mp_teNameNewTimeline	= 0;
	mp_btnNewTimeline		= 0;
	mp_ddConfigTimeline		= 0;
	mp_btnLoadTimeline		= 0;
	mp_tgPlayWithTimecode	= 0;
	mp_lblScriptConsole		= 0;

	m_timelineCurrentFolder = getConfigPath("config01");
	m_bAutoplay 			= false;
	
	m_bPlayWithTimecode		= false;

	classProperty_bool* pPropPlayPause = new classProperty_bool("bPlayPause",0,classProperty_bool::MODE_BUTTON);
	m_properties.add( pPropPlayPause );
	classProperty_bool* pPropStop = new classProperty_bool("bStop",0,classProperty_bool::MODE_BUTTON);
	m_properties.add( pPropStop );
	
	setMidiName("tool Timeline");
}

//--------------------------------------------------------------
void toolTimeline::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float dim = 16;
		int widthDefault = 320;

		// Colonne 1
	    mp_canvas->addWidgetDown( new ofxUILabel("Timeline",OFX_UI_FONT_LARGE) );
	  	mp_canvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );

		mp_btnPlayPauseTimeline = new ofxUILabelButton("Play", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetDown( mp_btnPlayPauseTimeline );

		mp_btnStopTimeline = new ofxUILabelButton("Stop", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetRight( mp_btnStopTimeline );

		mp_tgAutoplayTimeline = new ofxUIToggle("Loop", false, dim, dim);
		mp_canvas->addWidgetRight( mp_tgAutoplayTimeline );

		mp_tgLoopTimeline = new ofxUIToggle("Autoplay", false, dim, dim);
		mp_canvas->addWidgetRight( mp_tgLoopTimeline );

		mp_teDurationTimeline = new ofxUITextInput("Duration", "60", 100, dim);
		mp_teDurationTimeline->setAutoClear(false);
		mp_teDurationTimeline->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);

		mp_canvas->addWidgetDown( mp_teDurationTimeline );

		mp_canvas->addWidgetRight( new ofxUILabel("Duration",OFX_UI_FONT_SMALL) );

		mp_btnSaveTimeline = new ofxUILabelButton("Save", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetDown( mp_btnSaveTimeline );



	    mp_canvas->addWidgetDown(new ofxUILabel("Configuration", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1) );

		mp_teNameNewTimeline	= new ofxUITextInput("NameConfig", "", 200, dim);
		mp_teNameNewTimeline->setAutoClear(false);
		mp_teNameNewTimeline->setTriggerType(OFX_UI_TEXTINPUT_ON_ENTER);

		mp_btnNewTimeline		= new ofxUILabelButton("Create", false, 100,dim,0,0,OFX_UI_FONT_SMALL);

		vector<string> timelineConfigs = getTimelineConfigNamesList();

		mp_ddConfigTimeline		= new ofxUIDropDownList("Configs", timelineConfigs, 200,0,0,OFX_UI_FONT_SMALL);
		mp_ddConfigTimeline->setAutoClose(true);
		mp_btnLoadTimeline		= new ofxUILabelButton("Load", false, 100,dim,0,0,OFX_UI_FONT_SMALL);

		mp_canvas->addWidgetDown( mp_teNameNewTimeline );
		mp_canvas->addWidgetRight( mp_btnNewTimeline );
		mp_canvas->addWidgetDown( mp_ddConfigTimeline );
		mp_canvas->addWidgetRight( mp_btnLoadTimeline );


	    mp_canvas->addWidgetDown(new ofxUILabel("Timecode", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1) );
		mp_tgPlayWithTimecode = new ofxUIToggle("Play with timecode", &m_bPlayWithTimecode, dim, dim);
		mp_canvas->addWidgetDown( mp_tgPlayWithTimecode );


	    mp_canvas->addWidgetDown(new ofxUILabel("Transitions", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1) );
		mp_canvas->addWidgetDown(new ofxUISlider("transition duration (s)", 0.1f, 1.0f, 0.15f, widthDefault, dim ));

	    mp_canvas->addWidgetDown(new ofxUILabel("Script", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1) );

		ofxUILabelButton* pBtnReloadScript = new ofxUILabelButton("Reload script", false, 100,dim,0,0,OFX_UI_FONT_SMALL);
		mp_canvas->addWidgetDown( pBtnReloadScript );
		
		mp_lblScriptConsole = new ofxUILabel("lblScriptConsole", OFX_UI_FONT_SMALL);
    	mp_canvas->addWidgetDown(mp_lblScriptConsole);
		mp_lblScriptConsole->setLabel("");





		mp_canvas->autoSizeToFitWidgets();

		ofAddListener(m_properties.getBool("bPlayPause")->onValueChanged, this, &toolTimeline::onPlayPause);
		ofAddListener(m_properties.getBool("bStop")->onValueChanged, this, &toolTimeline::onStop);

	}

	// Timeline UI
	createTimeline();
}

//--------------------------------------------------------------
void toolTimeline::setup()
{
	OFAPPLOG->begin("toolTimeline::setup()");
	OFAPPLOG->begin(" - autoplay = " + ofToString(m_bAutoplay));

	if (m_bPlayWithTimecode == false)
	{
		if (m_bAutoplay)
		{
			m_timeline.play();
			updateLayout();
		}
	}
	else
	{
		m_bAutoplay = false;
	}
	
	loadScript("timeline.js");
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void toolTimeline::loadData()
{
	tool::loadData();

	if (mp_teNameNewTimeline)
		m_timelineCurrentFolder = getConfigPath(mp_teNameNewTimeline->getTextString());
	
	m_timeline.loadTracksFromFolder(m_timelineCurrentFolder);
	// Load extra data
	OFAPPLOG->println("- m_timelineCurrentFolder="+m_timelineCurrentFolder );
	string extraDataFilename = m_timelineCurrentFolder+"infos.xml";
	ofxXmlSettings extraXml;
	if (extraXml.load(extraDataFilename))
	{
		OFAPPLOG->println("- OK loaded "+extraDataFilename );
		m_timeline.setDurationInSeconds( extraXml.getValue("duration", 60.0) );
		updateLayout();
	}

	OFAPPLOG->println("- m_timeline duration="+ofToString( m_timeline.getDurationInSeconds() ) );
	ofAddListener(m_timeline.events().bangFired, this, &toolTimeline::bangFired);

	OFAPPLOG->end();


	// Because text input does not trigger event when loading
/*	if (mp_teDurationTimeline)
	{
		float duration = ofToFloat( mp_teDurationTimeline->getTextString() );
		m_timeline.setDurationInSeconds(duration);
	}
*/

}

//--------------------------------------------------------------
void toolTimeline::createTimeline()
{
	OFAPPLOG->begin("toolTimeline::createTimeline");
	// Reset
	ofRemoveListener(m_timeline.events().bangFired, this, &toolTimeline::bangFired);
	m_timeline.reset();

	// Create
	m_timeline.setSpacebarTogglePlay(false);
	m_timeline.setup();
	m_timeline.setWorkingFolder(m_timelineCurrentFolder);
	m_timeline.setAutosave(false);


//	m_timeline.addFlags("setAnim");
//	m_timeline.addPage("Animations");
	m_timeline.addFlags("scripts");

	m_timeline.addPage("Devices");
	
	
	m_timeline.setCurrentPage(0);
	

}

//--------------------------------------------------------------
void toolTimeline::update()
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
				m_timeline.setCurrentPage("Devices");
				pDeviceColorTrack = m_timeline.addColors(ofxTLTrackName);
				m_timeline.loadTracksFromFolder(m_timelineCurrentFolder);

			 }
			
			if (pDeviceColorTrack)
			{
				ofColor colorDevice = ((ofxTLColorTrack*)pDeviceColorTrack)->getColor();
				pDeviceManager->m_listDevices[i]->setColorHueSaturation( colorDevice.getHue(), colorDevice.getSaturation());
			//pDeviceColorTrack->setDrawRect(ofRectangle(0,0, 200, 50));
			}
		}
	}

	if (m_bPlayWithTimecode)
	{
		m_timeline.setCurrentTimeMillis( toolMidi::sm_timecodeMillis );
	}
}

//--------------------------------------------------------------
void toolTimeline::updateLayout()
{
	if (mp_btnPlayPauseTimeline)
		mp_btnPlayPauseTimeline->getLabelWidget()->setLabel( m_timeline.getIsPlaying() && !m_bPlayWithTimecode ? "Pause" : "Play" );
	if (mp_teDurationTimeline)
		mp_teDurationTimeline->setTextString( ofToString( m_timeline.getDurationInSeconds() ) );
	if (mp_lblScriptConsole)
		mp_lblScriptConsole->setLabel(m_errorScript);
}

//--------------------------------------------------------------
void toolTimeline::drawUI()
{
	tool::drawUI();
	float margin = 10;
	m_timeline.setWidth(ofGetWidth()-2*margin);
	m_timeline.setOffset(ofVec2f(margin,ofGetHeight()-m_timeline.getHeight()-margin));
	m_timeline.draw();
}


//--------------------------------------------------------------
void toolTimeline::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	if (name == "Save")
	{
		OFAPPLOG->begin("toolTimeline::handleEvents() - SAVE");
	
	
	
		m_timeline.saveTracksToFolder(m_timelineCurrentFolder);
		m_timeline.save();

		string extraDataFilename = m_timelineCurrentFolder+"infos.xml";
		ofxXmlSettings extraXml;
		extraXml.setValue("duration", m_timeline.getDurationInSeconds());
		bool okExtra = extraXml.save(extraDataFilename);

	   	OFAPPLOG->println(" - saving extra data to '"+extraDataFilename+"', OK ? "+ (okExtra ? "YES" : "NO"));
	
		OFAPPLOG->end();


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
	else if (name == "Duration")
	{
		if (mp_teDurationTimeline)
		{
			float duration = ofToFloat( mp_teDurationTimeline->getTextString() );
			m_timeline.setDurationInSeconds(duration);
		}
/*		int triggerType  = mp_teDurationTimeline->getInputTriggerType();
		if (triggerType == OFX_UI_TEXTINPUT_ON_ENTER)
		{
		}
*/
	}
	else if (name == "NameConfig")
	{
		int triggerType  = mp_teNameNewTimeline->getInputTriggerType();
		if (mp_teNameNewTimeline)
			m_timelineConfigName = mp_teNameNewTimeline->getTextString();
/*		if (triggerType == OFX_UI_TEXTINPUT_ON_ENTER)
		{
		}
*/
	}
	else if (name == "Create")
	{
		ofxUIButton* pBtn = e.getButton();
		if (pBtn->getValue()){
			createNewTimeline();
		}
	}
	else if (name == "Configs")
	{
	}
	else if (name == "Load")
	{
		vector<string> selected = mp_ddConfigTimeline->getSelectedNames();
		if (selected.size()>0)
		{
			OFAPPLOG->begin("toolTimeline::handleEvents() - LOAD");
			
			m_timelineCurrentFolder = getConfigPath( selected[0] );
			OFAPPLOG->println(" - m_timelineCurrentFolder = "+m_timelineCurrentFolder);
			
			createTimeline();
			updateLayout();
			
			// update layout : Specific to this action
			mp_ddConfigTimeline->setLabelText( selected[0] );
			if (mp_teNameNewTimeline) mp_teNameNewTimeline->setTextString( selected[0] );

			OFAPPLOG->end();
		}
	}
	else if (name == "Reload script")
	{
		ofxUIButton* pBtn = e.getButton();
		if (pBtn->getValue()){
			loadScript("timeline.js");
		}
	}
}

//--------------------------------------------------------------
void toolTimeline::bangFired(ofxTLBangEventArgs& args)
{
	string trackName = args.track->getName();
	if (trackName == "setAnim")
	{
		AnimationManager& animManager = GLOBALS->getSurfaceMain()->getAnimationManager();

		string animationNameWithArgs = args.flag;
		vector<string> animParts = ofSplitString(animationNameWithArgs, "/");

		if (animParts.size() == 1)
		{
			if (animParts[0] == "waves2") animParts[0] += ".js";
			animManager.M_setAnimation( animParts[0] );
		}
		else{
			if (animParts[1] == "d")
				animManager.M_setAnimationDirect( animParts[0] );
			else
				animManager.M_setAnimation( animParts[0] );
		}
	}
	else if (trackName == "scripts")
	{
		string jsFunction = "timeline_"+args.flag;
		// Simply call the function with timeline prefixed
		ofxJSValue retVal;
		ofxJSCallFunctionNameGlobal_NoArgs(jsFunction.c_str(),retVal);
	}
}

//--------------------------------------------------------------
void toolTimeline::onPlayPause(bool& value)
{
	m_timeline.togglePlay();
	updateLayout();
}

//--------------------------------------------------------------
void toolTimeline::onStop(bool& value)
{
	m_timeline.stop();
	m_timeline.setCurrentTimeSeconds(0);
	updateLayout();
}


//--------------------------------------------------------------
void toolTimeline::createNewTimeline()
{
	OFAPPLOG->begin("toolTimeline::createNewTimeline()");
	// Create dir
	m_timelineCurrentFolder = getConfigPath( m_timelineConfigName );

	ofDirectory dir(m_timelineCurrentFolder);
	if ( dir.create() )
	{
		OFAPPLOG->println("- created folder "+m_timelineCurrentFolder);
		createTimeline();
	}
	else
	{
		OFAPPLOG->println("- error creating dir "+m_timelineCurrentFolder);
	}
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
vector<string> toolTimeline::getTimelineConfigNamesList()
{
	OFAPPLOG->begin( "toolTimeline::getTimelineConfigNamesList()" );
	vector<string> names;

	ofDirectory dir(sm_timelineDir);
	dir.listDir();
	vector<ofFile> files = dir.getFiles();
	int nbFiles = files.size();
	OFAPPLOG->println("- "+ofToString(nbFiles)+" file(s)");
	for (int i=0; i<nbFiles; i++)
	{
		if (files[i].isDirectory())
		{
			names.push_back( files[i].getBaseName() );
		}
	}
	OFAPPLOG->end();
	
	return names;
}


//--------------------------------------------------------------
void toolTimeline::loadScript(string name)
{
	OFAPPLOG->begin("toolTimeline::loadScript("+name+")");
	
	// Script
	// Load globals
	ofxJSScript* pScript = ofxJSLoadFromData("Scripts/"+name, "timeline"); // "exampleId" is used for error reporting
	if (pScript){
		bool evalOk = ofxJSEval(pScript);
		if (evalOk){
			m_errorScript = "OK loaded "+name;
		}
		else
			m_errorScript = "Error evaluating the script "+name;
	}
	else
	{
		m_errorScript = "Error loading the script "+name;
	}
	updateLayout();

	OFAPPLOG->println(" - error="+m_errorScript);
	OFAPPLOG->end();
}





