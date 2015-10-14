/*
 *  animations.cpp
 *  versatubes
 *
 *  Created by 2Roqs on 30/04/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#include "animations.h"
#include "globals.h"
#include "utils.h"
#include "ofxJSScript.h"
#include "soundManager.h"


#define ANIM_UI_WIDTH_DEFAULT   330
#define ANIM_UI_HEIGHT_DEFAULT   16




#define ____________AnimationSoundPlayer____________
AnimationSoundPlayer::AnimationSoundPlayer()
{
	m_volume = 1.0f;
}

//--------------------------------------------------------------
void AnimationSoundPlayer::add(string name)
{
    m_listSoundNames.push_back( name );
}

//--------------------------------------------------------------
void AnimationSoundPlayer::playRandom(vector<int>& listSpeakers)
{
	// TODO : cache this!!!
	int nbSpeakers = listSpeakers.size();
	int speakers[nbSpeakers];
	for (int i=0;i<nbSpeakers;i++){
		speakers[i] = listSpeakers.at(i);
	}
	
	playRandom(speakers, nbSpeakers);
}

//--------------------------------------------------------------
void AnimationSoundPlayer::playRandom(int* speakers, int nbSpeakers)
{
	// ofLog() << "playing random sounds with " << m_listSoundNames.size() << " sounds";

    if (m_listSoundNames.size()>=2)
    {
        int rndIndex = -1;
        do
        {
            rndIndex = floor(ofRandom(1.0) * m_listSoundNames.size());
            if (rndIndex == m_listSoundNames.size())
                rndIndex = 0;
        }
        while (rndIndex==m_lastPlayedIndex);
        m_lastPlayedIndex = rndIndex;

        SoundManager::instance()->playSound( m_listSoundNames[rndIndex],false,m_volume,speakers,nbSpeakers );
    }
    else
    if (m_listSoundNames.size()==1)
        SoundManager::instance()->playSound( m_listSoundNames[0],false,m_volume,speakers,nbSpeakers );

}

//--------------------------------------------------------------
void AnimationSoundPlayer::print(string title)
{
	ofLog() << "+AnimationSoundPlayer for " << title;
	for (int i=0; i<m_listSoundNames.size(); i++){
		ofLog() << " - " << m_listSoundNames[i];
	}
}



#define ____________Animation____________
std::map<JSObject*, Animation*> Animation::sm_mapJSObj_Anim;

//--------------------------------------------------------------
Animation::Animation() : midiInterface()
{
	M_zeroAll();
}

//--------------------------------------------------------------
Animation::Animation(string name_) : midiInterface()
{
	M_zeroAll();
	m_name			= name_;
	setMidiName(m_name);
	populateConfigurations();
}

//--------------------------------------------------------------
Animation::Animation(string name_,string pathAbsScript_) : midiInterface()
{
	M_zeroAll();
	m_name			= name_;
	m_pathAbsScript = pathAbsScript_;

	mp_obj = JS_NewObject(ofxJSGetContext(), NULL, NULL, ofxJSGetGlobalObj());
    JSBool ok = JS_SetPrivate(ofxJSGetContext(), mp_obj, (void*)this);
	JS_AddNamedRoot(ofxJSGetContext(), &mp_obj, m_name.c_str());
    
    
    ofxJSDefineFunctionObject(mp_obj, "newSlider", Animation::jsNewSlider, 4);
    ofxJSDefineFunctionObject(mp_obj, "newToggle", Animation::jsNewToggle, 4);


    sm_mapJSObj_Anim[mp_obj] = this;

    // printf("#### setting private %s for obj %p, this=%p\n", ok == JS_TRUE ? "OK" : "NO", mp_obj, this);
	setMidiName(m_name);
	populateConfigurations();
}

//--------------------------------------------------------------
Animation::~Animation(){
//	printf("~Animation()\n");
	delete mp_script;
    delete mp_UIcanvas;
}

//--------------------------------------------------------------
void Animation::M_zeroAll()
{
	mp_script		= 0;
	mp_obj			= 0;
	m_isAutoClear	= true;
    mp_UIcanvas     = 0;
	mp_UIcanvas01	= 0;
	mp_UIVolumeTh	= 0;
	m_volValuesMeanTh = 0.5f;
	mp_lblVolValues = 0;
	mp_teConfigName = 0;
	
	m_isColor				= false;
	m_isColorFromDevice		= false;
	
	m_bResetOnEnter = false;
	
	setDrawBackground(true);
}


//--------------------------------------------------------------
bool Animation::M_loadScript(const char* s)
{
	OFAPPLOG->begin("Animation::M_loadScript");
//	printf("M_loadScript() > path='%s'\n", s);
	string pathAbs = s;
	if (pathAbs != "")
	{
		mp_script = ofxJSLoad(pathAbs,"___tmpScript___");
		if (mp_script)
		{
			if (ofxJSEvalOnObject(mp_script, mp_obj))
			{
				OFAPPLOG->println("OK evaluated '"+ofToString(s)+"'");
				OFAPPLOG->end();
				return true;
			}
			else{
				OFAPPLOG->println("ERROR cannot eval script '"+ofToString(s)+"'");
				M_deleteScript();
			}
		}
		else{
			OFAPPLOG->println("ERROR cannot load script '"+ofToString(s)+"'");
			M_deleteScript();
		}
	}
	else
	{
		OFAPPLOG->println("ERROR cpath for script '"+ofToString(s)+"' is empty..." );
	}

	OFAPPLOG->end();
	return false;
}

//--------------------------------------------------------------
bool Animation::M_reloadScript()
{
	return M_loadScript(m_pathAbsScript.c_str());
}

//--------------------------------------------------------------
void Animation::VM_setArgs(string args)
{
	loadConfiguration(args);
}

//--------------------------------------------------------------
void Animation::createUI()
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->addWidgetDown(new ofxUILabel(m_name, OFX_UI_FONT_LARGE));
        mp_UIcanvas->addWidgetDown(new ofxUISpacer(ANIM_UI_WIDTH_DEFAULT, 2));
    }
    
    if (mp_UIcanvas && mp_obj)
    {
        ofxJSValue retVal;
        ofxJSCallFunctionNameObject_NoArgs_IfExists(mp_obj,"createUI",retVal);
    }
 
	createUIVolume();
	createUIConfiguration();
	createUIMidi();
	createUISound();
    createUICustom();

    ofAddListener(mp_UIcanvas->newGUIEvent, this, &Animation::guiEvent);
}

//--------------------------------------------------------------
void Animation::addUISlider(classProperty_float* pProp)
{
    if (mp_UIcanvas)
        mp_UIcanvas->addSlider(pProp->m_name, pProp->m_min, pProp->m_max, pProp->mp_variable);
}

//--------------------------------------------------------------
void Animation::addUISlider(classProperty_int* pProp)
{
    if (mp_UIcanvas)
        mp_UIcanvas->addIntSlider(pProp->m_name, pProp->m_min, pProp->m_max, pProp->mp_variable);
}

//--------------------------------------------------------------
void Animation::addUItoggle(classProperty_bool* pProp)
{
    if (mp_UIcanvas)
        mp_UIcanvas->addToggle(pProp->m_name, pProp->mp_variable);
}

//--------------------------------------------------------------
void Animation::registerSoundTags(vector<string>& soundTags)
{
	soundTags.push_back("hit");
}

//--------------------------------------------------------------
void Animation::playSound(string deviceId)
{
	Device* pDevice = getDevice(deviceId);
	if (pDevice)
	{
		m_soundPlayer.playRandom(pDevice->m_listSpeakerIds);
	}
}


//--------------------------------------------------------------
void Animation::createUIVolume()
{
	if (mp_UIcanvas)
	{
		vector<float> zeros;
		zeros.assign(300, 0.0f);
		mp_UIVolumeTh = new ofxUIMovingGraphThreshold(ANIM_UI_WIDTH_DEFAULT,100, zeros, 300, 0.0f, 1.0f, "volValues");

		mp_lblVolValues = new ofxUILabel("lblVolValues", "values for device ##",OFX_UI_FONT_SMALL);
		mp_UIcanvas->addWidgetDown( mp_lblVolValues );
		mp_UIcanvas->addWidgetDown( mp_UIVolumeTh );
		mp_UIcanvas->addWidgetDown( new ofxUISlider("volValuesMeanTh", 0.0f,1.0f, &m_volValuesMeanTh, ANIM_UI_WIDTH_DEFAULT, 16) );
	}
}
//--------------------------------------------------------------
void Animation::createUIConfiguration()
{
	if (mp_UIcanvas == 0) return;

	mp_UIcanvas->addWidgetDown(new ofxUILabel("Configurations", OFX_UI_FONT_MEDIUM));
    mp_UIcanvas->addWidgetDown(new ofxUISpacer(ANIM_UI_WIDTH_DEFAULT, 2));

	mp_teConfigName = new ofxUITextInput("teConfigName", "", 200, 16);
	mp_teConfigName->setAutoUnfocus(true);
	mp_teConfigName->setAutoClear(false);
	
	mp_UIcanvas->addWidgetDown(mp_teConfigName);
	mp_UIcanvas->addWidgetRight(new ofxUILabelButton("Save", 100, false, OFX_UI_FONT_SMALL));
	mp_UIcanvas->addWidgetRight(new ofxUILabelButton("Load", 100, false, OFX_UI_FONT_SMALL));
	
	ofxUIToggle* pTgReset = new ofxUIToggle("resetOnEnter", &m_bResetOnEnter, 16,16);
    mp_UIcanvas->addWidgetDown(pTgReset);
	pTgReset->getLabelWidget()->setLabel("reset on enter");
	
}

//--------------------------------------------------------------
void Animation::createUIMidi()
{
	if (mp_UIcanvas == 0) return;
	mp_UIcanvas->addWidgetDown(new ofxUILabel("Midi", OFX_UI_FONT_MEDIUM));
    mp_UIcanvas->addWidgetDown(new ofxUISpacer(ANIM_UI_WIDTH_DEFAULT, 2));

	ofxUILabelButton* pBtn = new ofxUILabelButton("exportTemplate", false, ANIM_UI_WIDTH_DEFAULT, 16, 0,0, OFX_UI_FONT_SMALL);
	mp_UIcanvas->addWidgetDown( pBtn );
	pBtn->getLabelWidget()->setLabel("Export template");
}

//--------------------------------------------------------------
void Animation::createUISound()
{
	if (mp_UIcanvas == 0) return;
	
	mp_UIcanvas->addWidgetDown(new ofxUILabel("Sounds", OFX_UI_FONT_MEDIUM));
    mp_UIcanvas->addWidgetDown(new ofxUISpacer(ANIM_UI_WIDTH_DEFAULT, 2));

	// Soundplayer properties
	mp_UIcanvas->addWidgetDown(new ofxUISlider("volume", 0.0f, 1.0f, 1.0f,310,ANIM_UI_HEIGHT_DEFAULT));
	
	// Sound lists (filtered by tags)
	vector<string> soundTags;
	soundTags.push_back(m_name);
	registerSoundTags(soundTags);
	
	string tags="";
	string tags_sep="";
	for (int i=0;i<soundTags.size();i++){
		tags += tags_sep+"'"+soundTags[i]+"'";
		tags_sep=", ";
	}
	
	
	m_listSoundNames = SoundManager::instance()->getListSoundsNameWithTag(soundTags);
	int nbSounds = m_listSoundNames.size();
	for (int i=0; i<nbSounds; i++)
	{
		ofxUIToggle* pSoundToggle = new ofxUIToggle(m_listSoundNames[i],false,20,20);
		if (i%4 == 0)
			mp_UIcanvas->addWidgetDown(pSoundToggle);
		else
			mp_UIcanvas->addWidgetRight(pSoundToggle);
	}

	mp_UIcanvas->addWidgetDown(new ofxUITextArea("tags","Sound tags are "+tags+".",320,30,0,0,OFX_UI_FONT_SMALL));
}

//--------------------------------------------------------------
ofxUICanvas* Animation::getUI()
{
	return mp_UIcanvas;
}

//--------------------------------------------------------------
ofxUICanvas* Animation::showUI(bool is)
{
    if (mp_UIcanvas){
        mp_UIcanvas->setVisible(is);
		// OFAPPLOG->println(" animation::showUI("+ofToString(is)+") for '"+this->m_name+"'");
    }
    return mp_UIcanvas;
}

//--------------------------------------------------------------
ofxUICanvas* Animation::hideUI()
{
    return showUI(false);
}

//--------------------------------------------------------------
bool Animation::guiEventTogglesSound(string name)
{
	bool isSoundToggle = false;
	vector<string> listSoundNames = SoundManager::instance()->getListSoundsName();
	for (int i=0;i<listSoundNames.size();i++){
		if (name == listSoundNames[i]){
			isSoundToggle = true;
			break;
		}
	}
	return isSoundToggle;
}


//--------------------------------------------------------------
void Animation::jsCallSoundChanged()
{
  if (mp_obj)
  {
	  ofxJSValue retVal;
	  ofxJSCallFunctionNameObject_NoArgs_IfExists(mp_obj, "soundsChanged", retVal);
  }
}

//--------------------------------------------------------------
Device* Animation::getDevice(string deviceId)
{
    DeviceManager* pDeviceManager = GLOBALS->mp_deviceManager;
	if (pDeviceManager)
	{
		return pDeviceManager->getDeviceById(deviceId);
	}
	return 0;
}

//--------------------------------------------------------------
Device* Animation::getDeviceCurrent()
{
    DeviceManager* pDeviceManager = GLOBALS->mp_deviceManager;
	if (pDeviceManager)
	{
		return pDeviceManager->getDeviceCurrent();
	}
	return 0;
}


//--------------------------------------------------------------
bool Animation::updateDevicePosition(string deviceId, float x, float y)
{
	bool is = false;
	if (m_devicePositions.find(deviceId) == m_devicePositions.end())
	{
		m_devicePositions[deviceId] = ofVec2f(x,y);
		is = true;
	}
	else
	{
		ofVec2f posNow = m_devicePositions[deviceId];
		if (posNow.x != x || posNow.y != y){
			is = true;
			m_devicePositions[deviceId].set(x,y);
		}
	}

	return is;
}


//--------------------------------------------------------------
void Animation::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	


    if (kind == OFX_UI_WIDGET_SLIDER_H || kind == OFX_UI_WIDGET_SLIDER_V)
    {
		if (name == "volume")
		{
			m_soundPlayer.setVolume( ((ofxUISlider*) e.widget)->getScaledValue() );
		}
		else
		{
			if (mp_obj)
    	    {
			    ofxJSValue retVal;
        	    ofxJSValue args[2];
            	args[0] = string_TO_ofxJSValue( name );
            	args[1] = float_TO_ofxJSValue( ((ofxUISlider*) e.widget)->getScaledValue() );
				ofxJSCallFunctionNameObject_IfExists(mp_obj,"eventUI", args,2,retVal);
			}
		}
    }
	else
	if (kind == OFX_UI_WIDGET_TOGGLE)
	{
		// TODO : not cool here

		if (guiEventTogglesSound(name))
		{
			m_soundPlayer.m_listSoundNames.clear();
			for (int i=0; i<m_listSoundNames.size(); i++){
				ofxUIToggle* pSoundToggle = (ofxUIToggle*) mp_UIcanvas->getWidget( m_listSoundNames[i] );
				if (pSoundToggle->getValue()){
//					 ofLog() << m_name << " / " << m_listSoundNames[i];
					m_soundPlayer.add( m_listSoundNames[i] );
				}
			}
		}
		else
		{
			if (mp_obj)
    	    {
				bool valToggle = ((ofxUIToggle*) e.widget)->getValue() ;

			    ofxJSValue retVal;
	            ofxJSValue args[2];
    	        args[0] = string_TO_ofxJSValue( name );
        	    args[1] = int_TO_ofxJSValue( valToggle ? 1 : 0 );
				ofxJSCallFunctionNameObject_IfExists(mp_obj,"eventUI", args,2,retVal);
			}
		}
	}
    else if (kind == OFX_UI_WIDGET_LABELBUTTON)
	{
		
		if ( e.getButton()->getValue())
		{
			if (name == "Save")
			{
				string filename = mp_teConfigName->getTextString();
				saveConfiguration(filename);

				m_configurationCurrent = filename;
			}
			else if (name == "Load")
			{
				string filename = mp_teConfigName->getTextString();
				m_configurationCurrent = filename;
				loadConfiguration(filename);
			}
			else if (name == "exportTemplate")
			{
				 exportMidiSettingsTemplate();
			}
		}
	}
}

//--------------------------------------------------------------
void Animation::saveProperties(string id)
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->saveSettings( getPropertiesFilename(id) );
    }
}


//--------------------------------------------------------------
void Animation::loadProperties(string id)
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->loadSettings( getPropertiesFilename(id) );
		
		// save current config name, not too good ...
		if (mp_teConfigName){
			m_configurationCurrent = mp_teConfigName->getTextString();
		}
    }
}

//--------------------------------------------------------------
string Animation::getPropertiesFilename(string id, bool isExtension)
{
    return "Config/animations/"+id+"_"+this->m_name+ ( isExtension ? ".xml" : "");
}

//--------------------------------------------------------------
void Animation::createDirConfiguration()
{
	ofDirectory dir( ofToDataPath("Config/animations/"+m_name) );
	bool bDirExists = dir.exists();

	if (bDirExists == false)
		bDirExists = dir.create();
}

//--------------------------------------------------------------
void Animation::populateConfigurations()
{
	OFAPPLOG->begin("Animation::populateConfigurations()");

	createDirConfiguration();

	ofDirectory dir( ofToDataPath("Config/animations/"+m_name) );
	
	if (dir.exists())
	{
		m_configurations.clear();
	
		dir.allowExt("xml");
		dir.listDir();
	 
		for(int i = 0; i < dir.numFiles(); i++)
		{
			OFAPPLOG->println("- filename = "+dir.getFile(i).getFileName());
		
			m_configurations.push_back(dir.getFile(i).getFileName());
		}
	}
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void Animation::saveConfiguration(string filename)
{
	createDirConfiguration();
    if (mp_UIcanvas)
    {
        mp_UIcanvas->saveSettings( ofToDataPath("Config/animations/"+m_name+"/"+filename) );
		populateConfigurations();
    }
}

//--------------------------------------------------------------
void Animation::loadConfiguration(string filename)
{
	OFAPPLOG->begin("Animation::loadConfiguration("+filename+")");
    if (mp_UIcanvas)
    {
		if (filename == "" && m_configurations.size()>0)
		{
			filename = m_configurations[0];
    	}
		//ofLog() << filename;
		
		mp_UIcanvas->loadSettings( ofToDataPath("Config/animations/"+m_name+"/"+filename) );
	}
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void Animation::loadConfigurationPrev()
{
	if (m_configurations.size()>1 && m_configurationCurrent!="")
	{
		int index = getConfigurationIndex(m_configurationCurrent);
		if (index>=0){
			index = index-1;
			if (index<0)
				index = m_configurations.size()-1;
			m_configurationCurrent = m_configurations[index];
			loadConfiguration(m_configurationCurrent);
		}
	}
}


//--------------------------------------------------------------
void Animation::loadConfigurationNext()
{
	if (m_configurations.size()>1 && m_configurationCurrent!="")
	{
		int index = getConfigurationIndex(m_configurationCurrent);
		if (index>=0){
			index = (index+1)%m_configurations.size();
			m_configurationCurrent = m_configurations[index];
			loadConfiguration(m_configurationCurrent);
		}
	}
}

//--------------------------------------------------------------
int Animation::getConfigurationIndex(string filename)
{
	int nbConfigurations = m_configurations.size();
	for (int i=0;i<nbConfigurations;i++)
	{
		if (m_configurations[i] == filename)
			return i;
	}
	return -1;
}



//--------------------------------------------------------------
ofxJSBOOL Animation::jsNewSlider(ofxJSContext* cx, ofxJSObject* obj, uintN argc, ofxJSValue* argv, ofxJSValue* retVal)
{
    
    Animation* pThis = sm_mapJSObj_Anim[obj];
//    printf(">>> obj=%p, pThis=%p, from the map %p\n", obj, pThis, sm_mapJSObj_Anim[obj]);
    if (pThis && pThis->mp_UIcanvas)
    {
        if (argc == 4)
        {
            string name = ofxJSValue_TO_string(argv[0]);
            float min = ofxJSValue_TO_float(argv[1]);
            float max = ofxJSValue_TO_float(argv[2]);
            float val = ofxJSValue_TO_float(argv[3]);

			// TODO : may be move this somewhere else ?
			classProperty_float* pProp = new classProperty_float(name,min,max);
			pThis->m_properties.add( pProp );

		 
            pThis->mp_UIcanvas->addWidgetDown( new ofxUISlider(name, min,max,pProp->mp_variable, ANIM_UI_WIDTH_DEFAULT,ANIM_UI_HEIGHT_DEFAULT) );

			
		 
            return JS_TRUE;
            
        }
    }
    
    return JS_FALSE;
}

//--------------------------------------------------------------
ofxJSBOOL Animation::jsNewToggle(ofxJSContext* cx, ofxJSObject* obj, uintN argc, ofxJSValue* argv, ofxJSValue* retVal)
{
    
    Animation* pThis = sm_mapJSObj_Anim[obj];
//    printf(">>> obj=%p, pThis=%p, from the map %p\n", obj, pThis, sm_mapJSObj_Anim[obj]);
    if (pThis && pThis->mp_UIcanvas)
    {
        if (argc == 2)
        {
            string name = ofxJSValue_TO_string(argv[0]);
            int val = ofxJSValue_TO_int(argv[1]);
            
            pThis->mp_UIcanvas->addWidgetDown( new ofxUIToggle(name, val == 0 ? false : true, ANIM_UI_HEIGHT_DEFAULT,ANIM_UI_HEIGHT_DEFAULT) );
        
            return JS_TRUE;
        }
    }
    
    return JS_FALSE;
}

//void Animation::jsNewSlider(string name, float min, float max, float val)
//{
//}


//--------------------------------------------------------------
void Animation::M_beginShader()
{
	m_shader.begin();
}

//--------------------------------------------------------------
void Animation::M_endShader()
{
	m_shader.end();
}

//--------------------------------------------------------------
void Animation::M_loadShader(string name)
{
	string v = "Shaders/"+name+".vert";
	string f = "Shaders/"+name+".frag";
	m_shader.load(v,f);
}



//--------------------------------------------------------------
void Animation::VM_enter()
{
	if (M_reloadScript())
	{
		ofxJSValue retVal;
		if (mp_obj){
			ofxJSCallFunctionNameObject_NoArgs_IfExists(mp_obj, "setup", retVal);
		}
	}
}

//--------------------------------------------------------------
void Animation::VM_exit()
{
	ofxJSValue retVal;
	if (mp_obj){
		ofxJSCallFunctionNameObject_NoArgs_IfExists(mp_obj, "exit", retVal);
	}

	M_deleteScript();	
}

//--------------------------------------------------------------
void Animation::M_deleteScript()
{
	if (mp_script){
		delete mp_script;
		mp_script=0;
	}
}


//--------------------------------------------------------------
void Animation::updateUIVolume()
{
	if (mp_UIcanvas && mp_UIVolumeTh)
	{
		mp_UIVolumeTh->setThreshold( m_volValuesMeanTh );
	
		Device* pDeviceCurrent = getDeviceCurrent();
		if (pDeviceCurrent)
		{

			if (mp_lblVolValues)
				mp_lblVolValues->setLabel("values for "+pDeviceCurrent->m_id);

			if (m_mapDeviceVolumAccum.find(pDeviceCurrent->m_id) != m_mapDeviceVolumAccum.end())
			{
				VolumeAccum* pVolumeAccum = m_mapDeviceVolumAccum[pDeviceCurrent->m_id];
				if (pVolumeAccum && pVolumeAccum->m_valueHistory.size()>0)
				{
					mp_UIVolumeTh->setBuffer( pVolumeAccum->m_valueHistory );
					mp_UIVolumeTh->setValue( pVolumeAccum->m_valueMean );

				}
			}
		}
	}
}

//--------------------------------------------------------------
void Animation::M_update(float dt)
{
	handleMidiMessages();
	VM_update(dt);
}


//--------------------------------------------------------------
void Animation::VM_update(float dt)
{
	if (mp_script)
	{
		// Reload
		if (Globals::instance()->m_isAutoReloadScript){
			m_tReloadScript += dt;
			if (m_tReloadScript>=1.0f){
				//M_reloadScript();
				// Check time stamp of file
				ofFile fileScript(m_pathAbsScript);
				Poco::File& pocoFileScript = fileScript.getPocoFile();
				Poco::Timestamp timestamp = pocoFileScript.getLastModified();
				if (timestamp > m_timestampFileScript){
					m_timestampFileScript = timestamp;
					M_reloadScript();
				}
			}
		}

		// Call update
		ofxJSValue retVal;
		ofxJSValue args[1];
		args[0] = float_TO_ofxJSValue(dt);
//		ofxJSCallFunctionNameGlobal_IfExists("update", args,1,retVal);

		if (mp_obj){
			ofxJSCallFunctionNameObject_IfExists(mp_obj,"update", args,1,retVal);
		}

	}
}

//--------------------------------------------------------------
void Animation::VM_draw(float w, float h)
{
	if (mp_script){
		ofxJSValue retVal;
		ofxJSValue args[2];
		args[0] = float_TO_ofxJSValue(w);
		args[1] = float_TO_ofxJSValue((h+1));
//		ofxJSCallFunctionNameGlobal_IfExists("draw", args,2,retVal);
		if (mp_obj){
			ofxJSCallFunctionNameObject_IfExists(mp_obj,"draw", args,2,retVal);
		}

	}
}

//--------------------------------------------------------------
void Animation::VM_doTransition(int which, float t)
{
//  SoundManager::instance()->setVolumeAll(t);
    
  if (mp_script && mp_obj)
  {
      ofxJSValue retVal;
      ofxJSValue args[2];
      args[0] = int_TO_ofxJSValue(which);
      args[1] = float_TO_ofxJSValue(t);

      ofxJSCallFunctionNameObject_IfExists(mp_obj,"doTransition", args,2,retVal);
  }
}

//--------------------------------------------------------------
void Animation::sOnVolumAccumEvent(void* pData, VolumeAccum* pVolumAccum)
{
	Animation* pThis = (Animation*)pData;
	if (pThis && pVolumAccum){
		 pThis->onVolumAccumEvent(pVolumAccum->m_deviceId);
	}
}

//--------------------------------------------------------------
void Animation::onVolumAccumEvent(string deviceId)
{
	playSound(deviceId);
}

//--------------------------------------------------------------
void Animation::accumulateVolume(float volume, string deviceId)
{
	// Volume Accumulators
	map<string,VolumeAccum*>::iterator it = m_mapDeviceVolumAccum.find(deviceId);
	VolumeAccum* pVolumeAccum = 0;
	if ( it == m_mapDeviceVolumAccum.end())
	{
		pVolumeAccum = new VolumeAccum();
		pVolumeAccum->m_deviceId = deviceId;
		pVolumeAccum->setTriggerInCb( sOnVolumAccumEvent, (void*) this);
		m_mapDeviceVolumAccum[deviceId] = pVolumeAccum;
		
		 ofLog() << "creating volumeAccum instance for "<<deviceId;
	}
	else{
		pVolumeAccum = it->second;
	}
	
	if (pVolumeAccum)
		pVolumeAccum->add( volume );
}


//--------------------------------------------------------------
void Animation::onNewPacket(DevicePacket* pPacket, string deviceId, float xNorm, float yNorm)
{
	// Scripting
	if (mp_script && mp_obj && pPacket)
    {
		ofxJSValue retVal;
		ofxJSValue args[4];
		args[0] = string_TO_ofxJSValue(deviceId);
		args[1] = float_TO_ofxJSValue(pPacket->m_volume);
		args[2] = float_TO_ofxJSValue(xNorm);
		args[3] = float_TO_ofxJSValue(yNorm);
        ofxJSCallFunctionNameObject_IfExists(mp_obj,"onNewPacket", args,4,retVal);
    }
}




//--------------------------------------------------------------
void Animation::loadColors()
{
	OFAPPLOG->begin("Animation::loadColors()");
	string pathColorSettings = "Config/animations/__colors__.xml";
	
	ofxXmlSettings colorSettings;


	if (colorSettings.load(pathColorSettings))
	{
		OFAPPLOG->println("- loaded "+pathColorSettings);
		colorSettings.pushTag("colors");
	    int nbColors = colorSettings.getNumTags("color");
	    OFAPPLOG->println("- nb colors="+ofToString(nbColors));
    	for (int i=0;i<nbColors;i++)
    	{
        	string colorHex = ofToLower(colorSettings.getValue("color", "#FFFFFF",i));
			ofColor color = ofColor::fromHex( ofHexToInt(colorHex) );


			OFAPPLOG->println("    - color["+ofToString(i)+"] = "+colorHex+"/"+ofToString(color));


			m_colors.push_back(color  );
		}

		colorSettings.popTag();
	}
	else
	{
		OFAPPLOG->println(OF_LOG_ERROR, "- error loading "+pathColorSettings);
	}
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
ofColor Animation::chooseRandomColor()
{
	if (m_colors.size()>0)
	{
		int index = (int) ofRandom(0, m_colors.size());
		if (index>=m_colors.size()) index = 0;
		return m_colors[index];
	}
	return ofColor(255);
}

//--------------------------------------------------------------
void Animation::loadMidiSettings()
{
	midiInterface::setClassProperties(&m_properties) ;
	midiInterface::loadMidiSettings();
}

//--------------------------------------------------------------
void Animation::handleMidiMessages()
{
	m_midiMutex.lock();
	int nbMidiMessages = m_midiMessagesToHandle.size();
	for (int i=0;i<nbMidiMessages;i++)
	{
		ofxMidiMessage& midiMessage = m_midiMessagesToHandle[i];

		int port = midiMessage.portNum;
		int control = midiMessage.control;
		
		midiPort* pMidiPort = getMidiPort(port);
		if (pMidiPort)
		{
		 if ( pMidiPort->m_mapMidiControlToProp.find( control ) != pMidiPort->m_mapMidiControlToProp.end() )
		 {
			 classProperty* pProp = pMidiPort->m_mapMidiControlToProp[control];

			 pProp->setValueFromMidiMessage( midiMessage );
			 onPropertyMidiModified(pProp);


			 // ————————————————————————————————————————————————————————————————————————————————
			 // Special actions if it is js
			 if (mp_obj && mp_UIcanvas)
			 {
				 // Update layout
				 if (pProp->m_type == classProperty::FLOAT)
				 {
					 ofxUISlider* pSliderProp = (ofxUISlider*) mp_UIcanvas->getWidget( pProp->m_name );
					 if (pSliderProp){
						 pSliderProp->setValue( *((classProperty_float*)pProp)->mp_variable) ;
						 mp_UIcanvas->triggerEvent(pSliderProp);
					 }
				 }
			 }
			 // ————————————————————————————————————————————————————————————————————————————————
		 }
		}
	}
	m_midiMessagesToHandle.clear();
	m_midiMutex.unlock();
}

//--------------------------------------------------------------
void  Animation::newMidiMessage(ofxMidiMessage& eventArgs)
{
	midiInterface::newMidiMessage(eventArgs);
}


#define ____________AnimationManager____________

//--------------------------------------------------------------
AnimationManager::AnimationManager()
{
	mp_animationCurrent = 0;	
	mp_animationNext	= 0;
	m_state				= STATE_PLAY;
	m_transitionDuration = 2.0f;
    m_animationInfoIndex = 0;
	//m_transitionTween.setFrameBasedAnimation(true);

}

//--------------------------------------------------------------
AnimationManager::~AnimationManager()
{
	vector<Animation*>::iterator it;
	for (it = m_listAnimations.begin(); it != m_listAnimations.end(); ++it){
		delete *it;
	}
}

//--------------------------------------------------------------
void AnimationManager::M_setAnimation(int index, string args)
{
	Animation* pAnimation = M_getAnimation(index);
	M_setAnimation(pAnimation, args);
}

//--------------------------------------------------------------
void AnimationManager::M_setAnimation(string name, string args)
{
	vector<Animation*>::iterator it;
	for (it = m_listAnimations.begin(); it != m_listAnimations.end(); ++it){
		if (name == (*it)->m_name){
			M_setAnimation(*it,args);
			break;
		}
	}
}

//--------------------------------------------------------------
void AnimationManager::M_setAnimation(Animation* pAnimation, string args)
{
	mp_animationNext = pAnimation;
	m_animationNextArgs = args;
	M_changeState(STATE_TRANSITION_OUT);
}

//--------------------------------------------------------------
void AnimationManager::M_setAnimationDirect(int index, string args)
{
	OFAPPLOG->begin("AnimationManager::M_setAnimationDirect()");
	OFAPPLOG->println("-index="+ofToString(index));
	OFAPPLOG->println("-state="+ofToString(m_state));
	Animation* pAnimation = M_getAnimation(index);
	if (pAnimation)
	{
		OFAPPLOG->println("-animation name='"+pAnimation->m_name+"'");
		M_setAnimationDirect(pAnimation->m_name, args);
	}
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void AnimationManager::M_setAnimationDirect(string name, string args)
{

	vector<Animation*>::iterator it;
	for (it = m_listAnimations.begin(); it != m_listAnimations.end(); ++it){
		if (name == (*it)->m_name){
			M_changeAnimation(*it, args);
			break;
		}
	}
}


//--------------------------------------------------------------
bool AnimationManager::M_isTransiting()
{
    return (m_state == STATE_TRANSITION_IN || m_state == STATE_TRANSITION_OUT);
}


//--------------------------------------------------------------
void AnimationManager::M_changeAnimation(Animation* pAnimationNext, string args)
{
    
	if (pAnimationNext)
	{
		if (mp_animationCurrent)
			mp_animationCurrent->VM_exit();

		mp_animationCurrent = pAnimationNext;
		mp_animationCurrent->VM_setArgs(args);
		mp_animationCurrent->VM_enter();
	}
}

//--------------------------------------------------------------
void AnimationManager::setAnimationNext()
{
    int indexAnimation = getAnimationIndex( mp_animationCurrent );
    if (indexAnimation>=0)
    {
        int nbAnimations = m_listAnimations.size();
        M_setAnimation( m_listAnimations[(indexAnimation+1)%nbAnimations] );
    }
}


//--------------------------------------------------------------
void AnimationManager::setAnimationTimelineNext()
{
    // Get animation info
    AnimationInfo animInfo = m_listAnimationsInfos[m_animationInfoIndex];

    // Launch animation
    Animation* pAnimation = M_getAnimationByName( animInfo.m_name );
    if (pAnimation)
        M_setAnimation( pAnimation );

    // For next time
    int nbAnimationsInfo = m_listAnimationsInfos.size();
    m_animationInfoIndex = (m_animationInfoIndex+1)%nbAnimationsInfo;
}

//--------------------------------------------------------------
void AnimationManager::gotoAnimationNext()
{
    int indexAnimation = getAnimationIndex( mp_animationCurrent );
    if (indexAnimation>=0)
    {
        int nbAnimations = m_listAnimations.size();
        Animation* animationNext = m_listAnimations[(indexAnimation+1)%nbAnimations];
        M_changeAnimation(animationNext,"");
    }
}

//--------------------------------------------------------------
void AnimationManager::gotoAnimationPrev()
{
    int indexAnimation = getAnimationIndex( mp_animationCurrent );
    if (indexAnimation>=0)
    {
        int nbAnimations = m_listAnimations.size();
        if (indexAnimation==0)
        {
            indexAnimation = nbAnimations;
        }
        Animation* animationPrev = m_listAnimations[indexAnimation-1];
        M_changeAnimation(animationPrev,"");
    }
}

//--------------------------------------------------------------
int AnimationManager::getAnimationIndex(Animation* pAnimation)
{
    for (int i=0; i<m_listAnimations.size(); i++)
		if (m_listAnimations[i] == pAnimation)
            return i;
    return -1;
}

//--------------------------------------------------------------
Animation* AnimationManager::M_getAnimation(int index)
{
	if (index < m_listAnimations.size())
		return m_listAnimations[index];
	return 0;
}

//--------------------------------------------------------------
Animation* AnimationManager::M_getAnimationByName(string name)
{
	vector<Animation*>::iterator it;
	for (it = m_listAnimations.begin(); it != m_listAnimations.end(); ++it){
		if ((*it)->m_name == name)
			return *it;
	}	
	return 0;
}

//--------------------------------------------------------------
Animation* AnimationManager::M_getAnimationByThemeRnd(int theme)
{
	int nb = m_listAnimationsTheme[theme]->size();
	if (nb>0)
	{
		int rnd = (int)ofRandom(0,nb);
		return m_listAnimationsTheme[theme]->at(rnd); 
	}	


/*	
	vector<Animation*>::iterator it;
	for (it = m_listAnimations.begin(); it != m_listAnimations.end(); ++it){
		if ((*it)->m_name == name)
			return *it;
	}	
	return 0;
*/
	return 0;
}


//--------------------------------------------------------------
void AnimationManager::M_addAnimation(Animation* pAnimation)
{
	m_listAnimations.push_back(pAnimation);
}

//--------------------------------------------------------------
string AnimationManager::M_getAnimationNameRnd()
{
	int index = (int)ofRandom(m_listAnimations.size()-1);
	return m_listAnimations[index]->m_name;
}

//--------------------------------------------------------------
bool AnimationManager::M_isAutoClear()
{
	if (mp_animationCurrent)
		return mp_animationCurrent->m_isAutoClear;
	return true;
}

//--------------------------------------------------------------
void AnimationManager::M_update(float dt)
{
	// Update current animation
	if (mp_animationCurrent){
		mp_animationCurrent->M_update(dt);
	}
		
	// Update transition tween
	if (m_state == STATE_TRANSITION_OUT || m_state == STATE_TRANSITION_IN)
    {
		m_transitionTween.update();
        if (mp_animationCurrent)
            mp_animationCurrent->VM_doTransition(m_state, m_transitionTween.getTarget(0));
    }
    
	// tween end ? 
	if (m_transitionTween.isCompleted())
	{
//        SoundManager::instance()->stopAll();
        
		if (m_state == STATE_TRANSITION_OUT) 
			M_changeState(STATE_TRANSITION_IN);
		else
		if (m_state == STATE_TRANSITION_IN) 
			M_changeState(STATE_PLAY);
	}
}

//--------------------------------------------------------------
void AnimationManager::M_drawCanvasBefore(float w, float h)
{
	if (mp_animationCurrent)
		mp_animationCurrent->VM_drawBefore(w,h);
}

//--------------------------------------------------------------
void AnimationManager::M_drawCanvas(float w, float h)
{
	// Draw animation
    ofPushStyle();
	if (mp_animationCurrent)
		mp_animationCurrent->VM_draw(w,h);
    M_drawTransition(w,h);
    ofPopStyle();
}


//--------------------------------------------------------------
void AnimationManager::M_drawTransition(float w, float h)
{
	// Draw transition
	if (m_state == STATE_TRANSITION_OUT || m_state == STATE_TRANSITION_IN)
	{
		// printf("tween=%.2f\n", m_transitionTween.getTarget(0));
	    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofPushStyle();
			ofSetColor(0,0,0, 255*m_transitionTween.getTarget(0));
			ofRect(0,0,w,h);
		ofPopStyle();
    	ofDisableBlendMode();
	}
}

//--------------------------------------------------------------
bool AnimationManager::M_editScript()
{
	if (mp_animationCurrent && mp_animationCurrent->M_isScript()){
		ofLVOpenProgram(mp_animationCurrent->m_pathAbsScript);
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool AnimationManager::M_reloadScript()
{
	if (mp_animationCurrent && mp_animationCurrent->M_isScript())
		return mp_animationCurrent->M_reloadScript();
	return false;
}

//--------------------------------------------------------------
void AnimationManager::M_readSettings(ofxXmlSettings& settings)
{
	OFAPPLOG->begin("AnimationManager::M_readSettings");

    settings.pushTag("surface");
    settings.pushTag("timeline");
    
    
    int nbAnimations = settings.getNumTags("animation");
    OFAPPLOG->println("nbAnimations="+ofToString(nbAnimations));
    for (int i=0;i<nbAnimations;i++)
    {
        string animName = settings.getValue("animation","",i);
        
        if (animName != "" && M_getAnimationByName(animName) != 0){
		    OFAPPLOG->println(animName);
            m_listAnimationsInfos.push_back( AnimationInfo(animName) );
        }
    }

    settings.popTag();
    settings.popTag();

	OFAPPLOG->end();
}

//--------------------------------------------------------------
void AnimationManager::M_changeState(int newState)
{
	if (m_state == STATE_PLAY)
	{
		if (newState == STATE_TRANSITION_OUT)
		{
			m_transitionTween.setParameters(1, easinglinear, ofxTween::easeOut, 0, 1, m_transitionDuration*1000.0f, 0);
			m_transitionTween.start();
			m_state = newState;
			
			// printf("STATE_PLAY > STATE_TRANSITION_OUT\n");
		}
	}
	else
	if (m_state == STATE_TRANSITION_OUT)
	{
		if (newState == STATE_TRANSITION_IN)
		{
			//if (mp_animationNext != mp_animationCurrent)
			if (mp_animationNext)
			{
				M_changeAnimation(mp_animationNext,m_animationNextArgs);
				mp_animationNext = 0;

				m_transitionTween.setParameters(1, easinglinear, ofxTween::easeOut, 1, 0, m_transitionDuration*1000.0f, 0);
				m_transitionTween.start();
			}
			m_state = newState;
		
			// printf("STATE_TRANSITION_OUT > STATE_TRANSITION_IN\n");
		}
	}
	else
	if (m_state == STATE_TRANSITION_IN)
	{
		if (newState == STATE_PLAY)
		{
			m_state = newState;
			// printf("STATE_TRANSITION_IN > STATE_PLAY\n");
		}
	}
}

//--------------------------------------------------------------
void AnimationManager::loadMidiSettings()
{
	int nbAnimations = m_listAnimations.size();
	for (int i=0;i<nbAnimations;i++)
	{
		m_listAnimations[i]->loadMidiSettings();
	}
}


//--------------------------------------------------------------
void AnimationManager::newMidiMessage(ofxMidiMessage& midiMessage)
{
	if (mp_animationCurrent)
	{
		mp_animationCurrent->newMidiMessage(midiMessage);
	}
}


