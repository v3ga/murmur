//
//  toolSound.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolSound.h"
#include "globals.h"
#include "testApp.h"
#include "soundManager.h"

//--------------------------------------------------------------
toolSound::toolSound(toolManager* parent) : tool("Sound", parent)
{
	
}
	
//--------------------------------------------------------------
void toolSound::createControlsCustom()
{
	if (mp_canvas)
	{
		float widthDefault = 320;
		float dim = 16;

	    mp_canvas->addWidgetDown(new ofxUILabel("Sounds", OFX_UI_FONT_LARGE));
	    mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    	mp_canvas->addWidgetDown(new ofxUIToggle( "activate main sound", GLOBALS->mp_app->isAnimationSequence, dim, dim));
		mp_canvas->addWidgetDown(new ofxUISlider( "vol. main sound min.", 0.0f, 1.0f, 0.5f, widthDefault-10, dim));
		mp_canvas->addWidgetDown(new ofxUISlider( "vol. main sound max.", 0.0f, 1.0f, 1.0f, widthDefault-10, dim));

//    mp_guiSound->addWidgetDown(new ofxUILabel("Library", OFX_UI_FONT_LARGE));
//    mp_guiSound->addWidgetDown(new ofxUISpacer(widthDefault, 2));

		ofxUIDropDownList* mp_ddlSounds = new ofxUIDropDownList("Library", SoundManager::instance()->getListSoundsName());
//	mp_ddlSounds->setLabelText("Sounds");
		mp_ddlSounds->open();
		mp_canvas->addWidgetDown(mp_ddlSounds);
		mp_canvas->autoSizeToFitWidgets();
	}
}

//--------------------------------------------------------------
void toolSound::update()
{
    SoundManager::instance()->update();
}


//--------------------------------------------------------------
void toolSound::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
 
	if (name == "activate main sound")
    {
        SoundManager::instance()->activateSoundMain( ((ofxUIToggle *) e.widget)->getValue() );
    }
    else if (name == "vol. main sound min.")
    {
        float value = ((ofxUISlider *) e.widget)->getScaledValue();
        SoundManager::instance()->m_soundMainVolumeMin = value;
        SoundManager::instance()->setVolumeSoundMain(value);
    }
    else if (name == "vol. main sound max.")
    {
        SoundManager::instance()->m_soundMainVolumeMax = ((ofxUISlider *) e.widget)->getScaledValue();
    }
    else if (name == "Library")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        if (selected.size()==1){
			//printf("selected %s", selected[0]->getName().c_str());
	    	
		}
    }
}
