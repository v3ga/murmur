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
	mp_canvasSound 		= 0;
	mp_lblSoundTitle 	= 0;
	mp_lblSoundInfos	= 0;
	mp_lblSoundInfos2	= 0;
	mp_teSoundTags		= 0;
	mp_soundCurrent		= 0;
	m_isUpdateLayout	= false;
}


//--------------------------------------------------------------
void toolSound::show(bool is)
{
	tool::show(is);
	if (mp_canvasSound)
		mp_canvasSound->setVisible(is);
}

//--------------------------------------------------------------
void toolSound::saveData()
{
	tool::saveData();
	ofxXmlSettings infos;
	infos.addTag("sounds");
	infos.pushTag("sounds");
	
	SoundManager* 	pSoundManager = SoundManager::instance();
	vector<string> 	soundNames = pSoundManager->getListSoundsName();

	for (int i=0;i<soundNames.size();i++)
	{
		SoundInfo*  pSoundInfo = pSoundManager->getSoundInfo( soundNames[i] );
		string tags = pSoundInfo ? pSoundInfo->getTagsString() : "";

		infos.addTag("sound");
		infos.setAttribute("sound", "file", soundNames[i], 	i);
		infos.setAttribute("sound", "tags", tags, 			i);
	}
	infos.popTag();

	if (infos.saveFile(getSoundInfosPathFile()))
	{


		ofLog() << "saved " << getSoundInfosPathFile();
	}
}

//--------------------------------------------------------------
/*
void logFilter(string name, vector<string>& list)
{
	ofLog() << "----- " << name;
	for (int i=0;i<list.size();i++)
	{
		ofLog() << " - " << list[i];
	}
}
*/
void toolSound::loadData()
{
	tool::loadData();
	ofxXmlSettings infos;
	if ( infos.loadFile(getSoundInfosPathFile()) )
	{
		// ofLog() << "loaded " << getSoundInfosPathFile();
		infos.pushTag("sounds");
			int nbSounds = infos.getNumTags("sound");
			// ofLog() << "    - nbSounds=" << ofToString(nbSounds);
			for (int i=0;i<nbSounds;i++)
			{
				string soundName = infos.getAttribute("sound", "file", "",i);
				if (soundName !="")
				{
					SoundInfo* pSoundInfo = new SoundInfo();
					pSoundInfo->setTagsString( infos.getAttribute("sound", "tags", "", i) );
					SoundManager::instance()->addSoundInfo( soundName, pSoundInfo );
					
					// ofLog() << "    - " << soundName << " "<< pSoundInfo->m_tags.size() << " tags ["<< pSoundInfo->getTagsString() <<"]";
				}
			}
		infos.popTag();
	}


/*
	vector<string> filter1_tags;
	filter1_tags.push_back( "toto1" );
	filter1_tags.push_back( "tutu1" );
	vector<string> filter1 = SoundManager::instance()->getListSoundsNameWithTag(filter1_tags);

	vector<string> filter2_tags;
	filter2_tags.push_back( "tutu1" );
	vector<string> filter2 = SoundManager::instance()->getListSoundsNameWithTag(filter2_tags);


	logFilter("filter1", filter1);
	logFilter("filter2", filter2);
*/

}



//--------------------------------------------------------------
string toolSound::getSoundInfosPathFile()
{
   return mp_toolManager->m_relPathData + "/" + "Sounds_infos.xml";
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

		
		string strDriverIndex=""; string sep="";
		for (int i=0;i<ofFmodGetNumDrivers();i++){ strDriverIndex+= sep+ofToString(i); sep=",";}

		mp_lblSoundInfos 	= new ofxUILabel("> Found " + ofToString(ofFmodGetNumDrivers()) + " sound driver(s): "+strDriverIndex, OFX_UI_FONT_SMALL);
		mp_lblSoundInfos2 	= new ofxUILabel("> Selected is " + ofToString( SoundManager::instance()->getDriverSelected() )+" (edit in configuration.xml)", OFX_UI_FONT_SMALL);

	    mp_canvas->addWidgetDown(mp_lblSoundInfos);
	    mp_canvas->addWidgetDown(mp_lblSoundInfos2);
	    mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 1));
	

    	mp_canvas->addWidgetDown(new ofxUIToggle( "activate main sound", GLOBALS->mp_app->isAnimationSequence, dim, dim));
		mp_canvas->addWidgetDown(new ofxUISlider( "vol. main sound min.", 0.0f, 1.0f, 0.5f, widthDefault-10, dim));
		mp_canvas->addWidgetDown(new ofxUISlider( "vol. main sound max.", 0.0f, 1.0f, 1.0f, widthDefault-10, dim));

		ofxUIDropDownList* mp_ddlSounds = new ofxUIDropDownList("Library", SoundManager::instance()->getListSoundsName(),widthDefault,0,0,OFX_UI_FONT_SMALL);
		mp_ddlSounds->open();
		mp_canvas->addWidgetDown(mp_ddlSounds);
		mp_canvas->autoSizeToFitWidgets();

     	ofxUIRectangle* pRectSoundList = mp_canvas->getRect();
        mp_canvasSound = new ofxUICanvas(pRectSoundList->getX()+pRectSoundList->getWidth()+10, pRectSoundList->getY(), widthDefault, 300);

		mp_lblSoundTitle = new ofxUILabel("", OFX_UI_FONT_LARGE);
		mp_teSoundTags = new ofxUITextInput("tags","",widthDefault,50);
		mp_canvasSound->addWidgetDown(mp_lblSoundTitle);
	    mp_canvasSound->addWidgetDown(new ofxUISpacer(widthDefault, 2));
		mp_canvasSound->addWidgetDown(mp_teSoundTags);
		mp_canvasSound->setVisible(false);

		mp_canvasSound->autoSizeToFitWidgets();
		ofAddListener(mp_canvasSound->newGUIEvent, this, &toolSound::handleEvents);
	}
}

//--------------------------------------------------------------
void toolSound::update()
{
    SoundManager::instance()->update();

	if (m_isUpdateLayout)
	{
		ofxUICanvas* pCanvas = getCanvas();
        mp_canvasSound->setPosition(pCanvas->getRect()->getX()+pCanvas->getRect()->getWidth()+10, pCanvas->getRect()->getY());

		if (mp_soundCurrent)
		{
			if (mp_lblSoundTitle) mp_lblSoundTitle->setLabel( mp_soundCurrent->m_name );
			if (mp_teSoundTags){
				SoundInfo* pSoundInfo = SoundManager::instance()->getSoundInfo(mp_soundCurrent->m_name);
				if (pSoundInfo)
					mp_teSoundTags->setTextString(pSoundInfo->getTagsString());
				else
					mp_teSoundTags->setTextString("");
			}

		}
	
		m_isUpdateLayout = false;
	}
}


//--------------------------------------------------------------
void toolSound::updateLayout()
{
	m_isUpdateLayout = true;
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
        if (selected.size()==1)
		{
			// Get sound current
			string soundName = selected[0]->getName();
			mp_soundCurrent = SoundManager::instance()->getSoundPlayer(soundName);
					
			// Update Layout
			updateLayout();
		}
    }
	else if (name == "tags")
	{
		ofxUITextInput* pTe = (ofxUITextInput*) e.widget;
		if (mp_soundCurrent)
		{
		   SoundInfo* pSoundInfo = SoundManager::instance()->getSoundInfo(mp_soundCurrent->m_name);
			if (pSoundInfo) pSoundInfo->setTagsString( pTe->getTextString() );
		}
		
	}
}
