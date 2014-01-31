#include "testApp.h"
#include "ofxJSGlobalFunc.h"
#include "globals.h"
#include "data.h"
#include "js.h"

#include "sceneVisualisation.h"
#include "deviceEchoSimulator.h"
#include "deviceNode.h"
#include "deviceInfo.h"
#include "soundInput.h"
#include "soundManager.h"
#include "animations.h"

//--------------------------------------------------------------
void testApp::setup()
{

    // Zero all
	m_isUpdateLayout = false;
	
	mp_gui1 = 0;
    mp_gui2 = 0;
    mp_gui3 = 0;
    mp_gui4 = 0;
    mp_guiAnimProps = 0;
    mp_ddlDevices = 0;
    mp_lblAnimTitle = 0;
    mp_lblDeviceTitle = 0;
    mp_tgViewSimu = 0;
    mp_sliderDeviceVolMax = 0;
    mp_sliderDeviceVolHistorySize = 0;
    mp_sliderDeviceVolHistoryTh = 0;
    mp_toggleDeviceEnableStandby = 0;
    mp_sliderDeviceTimeStandby = 0;
    mp_sliderDeviceNbLEDsStandby = 0;
    mp_sliderDeviceSpeedStandby = 0;
	mp_lblSurfaceActivity = 0;
    
    // Settings
    m_settings.loadFile("configuration.xml");
    int simulator = m_settings.getValue("murmur:simulator:enable",1);
    
    
    mp_deviceSimulator = 0;
    mp_deviceManager = new DeviceManager();
    mp_deviceInfoManager = new DeviceInfoManager();
    isFullscreen = false;
    isShowDevicePointSurfaces = false;
    isAnimationSequence = false;
    isViewSimulation = true;
    isSimulation =  simulator == 1 ? true : false;
    m_oscReceiverIP = "127.0.0.1";
    m_oscReceiverPort = 1234;
	m_isLaunchMadMapper = false;
	m_isLaunchMurmurRaspberry = false;
    
    // Globals
    Globals::instance()->mp_app = this;
    Globals::instance()->mp_oscReceiver = &m_oscReceiver;
//    Globals::instance()->mp_oscSender = &m_oscSender;
    Globals::instance()->mp_deviceManager = mp_deviceManager;
    Globals::instance()->mp_deviceInfoManager = mp_deviceInfoManager;
	
    // Initialize JS
    initJS();
    
    // Load Data
    Data::instance()->load();

    // Sounds
    SoundManager::instance()->setup();
    
    // Devices + Surfaces
    initDevices();
    initSurfaces();

    // Devices <> Surfaces
    attachDevicesToSurfaces();
    
    // Scenes
    initScenes();
    
	// Controls
	initControls();
    
    // Run
    m_oscReceiver.setup(m_oscReceiverPort);
    printf("Starting oscReceiver @port %d\n", m_oscReceiverPort);
	
	// Simulators
	initSimulators();

	// Launch MadMapper
	if (m_isLaunchMadMapper && !isSimulation)
	{
		launchMadMapper();
	}
	
	// Launch Murmur on Raspberry
	if (m_isLaunchMurmurRaspberry && !isSimulation)
	{
		launchMurmurRaspberry();
	}
	
	// GO
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::launchMurmurRaspberry()
{
	string strIPMurmur = m_settings.getValue("murmur:raspberry:ip", "10.23.108.114");
	string strRun = "ssh pi@" + strIPMurmur + " /home/pi/Dev/C/openFrameworks/examples/myapps/murmurRaspberry/bin/run_murmur.sh";

	m_threadRasp.setCommand(strRun);
	m_threadRasp.startThread();
}

//--------------------------------------------------------------
void testApp::launchMadMapper()
{

	string pathFileMM = m_settings.getValue("murmur:madmapper", "default.map");
	ofFile file(ofToDataPath("Config/madmapper/"+pathFileMM));
	if (file.exists())
	{
		string command = "open "+file.getAbsolutePath();
		system(command.c_str());
	   printf("Launching MadMapper with %s\n", file.getAbsolutePath().c_str());
	}
	else{
	   printf("Error launching MadMapper with %s\n", file.getAbsolutePath().c_str());
	}
}

//--------------------------------------------------------------
void testApp::exit()
{
	printf("[testApp::exit()]\n");
	
	
	m_soundStreamInput.stop();

    if (mp_deviceManager)
	{
        mp_deviceManager->saveDevicesXML("Config/devices/");
		mp_deviceManager->turnoffDevices();
	}

    if (mp_surfaceMain)
        mp_surfaceMain->saveAnimationsProperties();
        
	if (mp_gui1)
        mp_gui1->saveSettings("GUI/gui1_settings.xml");
    
	if (mp_gui4)
        mp_gui4->saveSettings("GUI/gui4_settings.xml");

    
    delete mp_sceneVisualisation;
    delete mp_deviceManager;
    delete mp_deviceInfoManager;
    delete Globals::instance();
    delete Data::instance();
	delete mp_gui1;
	delete mp_gui2;
	delete mp_gui3;
	delete mp_gui4;

	printf("[testApp::exit()] end\n");
}

//--------------------------------------------------------------
void testApp::initDevices()
{
	printf("[testApp::initDevices]\n");
    if (isSimulation)
    {
		m_settings.pushTag("murmur");
		m_settings.pushTag("simulator");
		m_settings.pushTag("devices");
		
		int nbDevicesSimulator = m_settings.getNumTags("device");
		printf("  - nbDevicesSimulator=%d\n", nbDevicesSimulator);
		for (int i=0; i<nbDevicesSimulator ; i++)
		{
			m_settings.pushTag("device", i);
		
	        int nbLEDs = m_settings.getValue("nbLEDs", 129);
    	    float length = m_settings.getValue("length", 4.0f);
			string id = m_settings.getValue("id", "deviceEchoSimulator");

			DeviceEchoSimulator* pDeviceSimulator = new DeviceEchoSimulator(id, nbLEDs, length/float(nbLEDs-1));
			m_listDeviceSimulator.push_back(pDeviceSimulator);
	        mp_deviceManager->addDevice(pDeviceSimulator);

			m_settings.popTag();

		}
		m_settings.popTag();
		m_settings.popTag();
		m_settings.popTag();
	
    }
}


//--------------------------------------------------------------
void testApp::initSimulators()
{
    if (isSimulation && m_listDeviceSimulator.size()>0 /* && mp_deviceSimulator*/)
    {
        int deviceSoundInputId = m_settings.getValue("murmur:simulator:soundInput:device", -1);
        int nbChannels = m_settings.getValue("murmur:simulator:soundInput:nbChannels", 2);
        printf("deviceSoundInputId=%d\n",deviceSoundInputId);
        printf("nbChannels=%d\n",nbChannels);

		m_soundStreamInput.listDevices();
	    if (deviceSoundInputId>=0)
		{
    	    m_soundStreamInput.setDeviceID(deviceSoundInputId);
			m_soundStreamInput.setup(ofGetAppPtr(), 0, nbChannels, 44100, 256, 4);
			m_soundStreamInput.start();
		}

		vector<DeviceEchoSimulator*>::iterator it = m_listDeviceSimulator.begin();
		for ( ; it != m_listDeviceSimulator.end() ; ++it)
		{
			DeviceEchoSimulator* pDeviceSim = *it;
			if (pDeviceSim){
//		        pDeviceSim->startSoundInput(deviceSoundInputId,nbChannels);
		        pDeviceSim->startSoundInput(nbChannels);
        		pDeviceSim->loadXML("Config/devices/");
				
				DeviceNode* pDeviceSimNode = mp_sceneVisualisation->getDeviceNode(pDeviceSim);
				if (pDeviceSimNode)
            		pDeviceSimNode->setPositionNodeSurface( mp_surfaceNodeMain->getGlobalPositionDevicePointSurface( pDeviceSim ) );
			}
		}

		// Select default
		selectDevice(m_listDeviceSimulator[0]->m_id);
		    
    }

}

//--------------------------------------------------------------
void testApp::initSurfaces()
{
    // TODO : should have a list here
    mp_surfaceMain = new Surface("main", ofGetWidth(),ofGetHeight());
    mp_surfaceMain->setup();
}

//--------------------------------------------------------------
void testApp::attachDevicesToSurfaces()
{
    // TEMP, should be somewhere in a config file
    if (mp_surfaceMain)
	{
        if (isSimulation)
		{
            //mp_surfaceMain->addDevice(mp_deviceSimulator);
			
        	vector<DeviceEchoSimulator*>::iterator it = m_listDeviceSimulator.begin();
	        for ( ; it != m_listDeviceSimulator.end() ; ++it)
        	{
            	DeviceEchoSimulator* pDeviceSim = *it;
				if (pDeviceSim)
				{
					mp_surfaceMain->addDevice(pDeviceSim);
				}
			}
		}
    }
}


//--------------------------------------------------------------
SurfaceNode* testApp::getSurfaceNode(Surface* pSurface)
{
	if (mp_sceneVisualisation)
		return mp_sceneVisualisation->getSurfaceNode(pSurface);
	return 0;
}

//--------------------------------------------------------------
Surface* testApp::getSurfaceForDevice(Device* pDevice)
{
    // TODO : iterate through a liste of surfaces to find the surface that has the device attached to it
    return mp_surfaceMain;
}

//--------------------------------------------------------------
Surface* testApp::getSurfaceForDeviceCurrent()
{
    return getSurfaceForDevice( mp_deviceManager->getDeviceCurrent() );
}

//--------------------------------------------------------------
void testApp::initScenes()
{
    // TEMP
    // Surface
    mp_surfaceNodeMain = new SurfaceNode( mp_surfaceMain,4.0,3.0 );
    mp_surfaceNodeMain->setPosition(-0.5f*mp_surfaceNodeMain->getWidth(),0,0);
 
    // Construct the scene
    mp_sceneVisualisation = new SceneVisualisation();
	mp_sceneVisualisation->loadSettings();
    mp_sceneVisualisation->setup();
    mp_sceneVisualisation->addSurfaceNode(mp_surfaceNodeMain);

    if (isSimulation)
	{
        // mp_sceneVisualisation->addDeviceNode(mp_deviceNodeSim, mp_surfaceNodeMain);
		vector<DeviceEchoSimulator*>::iterator it = m_listDeviceSimulator.begin();
		for ( ; it != m_listDeviceSimulator.end() ; ++it)
		{
			DeviceEchoSimulator* pDeviceSim = *it;
			if (pDeviceSim)
			{
				DeviceNode* pDeviceSimNode = new DeviceNode(pDeviceSim);
				//m_listDeviceSimNode.push_back( pDeviceSimNode );
	        	mp_sceneVisualisation->addDeviceNode(pDeviceSimNode, mp_surfaceNodeMain);
			}
		}
	
	}
	
    // Add to a list
    m_listScenes.push_back(mp_sceneVisualisation);
}

//--------------------------------------------------------------
void testApp::createDeviceNodeInScene(Device* pDevice)
{
    if (mp_sceneVisualisation)
    {
        mp_sceneVisualisation->createDeviceNode(pDevice, mp_surfaceNodeMain); // TODO : select another surface ?
    }
}

//--------------------------------------------------------------
void testApp::initControls()
{
	// UI
	float dim = 16;
	int widthDefault = 320;
	float h = 10;
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
	mp_gui1 = new ofxUICanvas(0,h,widthDefault,1200);
//	mp_gui1->addWidgetDown(new ofxUILabel("MURMUR", OFX_UI_FONT_LARGE));
    mp_gui1->addWidgetDown(new ofxUIImage(0,0,238,60,&Data::instance()->m_imgLogo,"logo",false));

//    mp_gui1->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_gui1->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_SMALL));
    
    mp_gui1->addWidgetDown(new ofxUILabel("CONFIGURATION", OFX_UI_FONT_MEDIUM));
    mp_gui1->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_gui1->addWidgetDown(new ofxUIToggle( dim, dim, isShowDevicePointSurfaces, "Show device points"));
    mp_tgViewSimu = new ofxUIToggle( dim, dim, isViewSimulation, "View simulation");
    mp_gui1->addWidgetRight(mp_tgViewSimu);
//    mp_gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "Show infos on projection"));
	mp_gui1->addWidgetDown(new ofxUISlider( widthDefault-10, dim, 0.0f, 50.0f, 10.0f, "cache LEDs diameter" ));

    mp_gui1->addWidgetDown(new ofxUILabel("OSC", OFX_UI_FONT_MEDIUM));
    mp_gui1->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_gui1->addWidgetDown(new ofxUILabel("port : ", OFX_UI_FONT_SMALL));
    mp_gui1->addWidgetRight(new ofxUITextInput("Port", ofToString(m_oscReceiverPort) , 100, dim,0,0,OFX_UI_FONT_SMALL));
    
    mp_gui1->addWidgetDown(new ofxUILabel("SOUND", OFX_UI_FONT_MEDIUM));
    mp_gui1->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_gui1->addWidgetDown(new ofxUIToggle( dim, dim, isAnimationSequence, "activate main sound"));
	mp_gui1->addWidgetDown(new ofxUISlider( widthDefault-10, dim, 0.0f, 1.0f, 0.5f, "vol. main sound min." ));
	mp_gui1->addWidgetDown(new ofxUISlider( widthDefault-10, dim, 0.0f, 1.0f, 1.0f, "vol. main sound max." ));
    
/*
    mp_gui1->addWidgetDown(new ofxUILabel("CONSOLE", OFX_UI_FONT_MEDIUM));
    mp_gui1->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    
    ofxUITextArea* mp_console = new ofxUITextArea( "console", "text\nexample", widthDefault-10, 200.0f, 0.0f,0.0f, OFX_UI_FONT_SMALL);
    mp_gui1->addWidgetDown(mp_console);
*/
    
    mp_gui1->autoSizeToFitWidgets();

    
	ofAddListener(mp_gui1->newGUIEvent, this, &testApp::guiEvent);
    mp_gui1->loadSettings("GUI/gui1_settings.xml");
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
	mp_gui2 = new ofxUICanvas(mp_gui1->getRect()->getWidth()+10,h,widthDefault+10,400);
    vector<string> listDevicesIds;
	mp_ddlDevices = new ofxUIDropDownList(widthDefault, "Devices", listDevicesIds, OFX_UI_FONT_LARGE);
    guiUpdateListDevices(widthDefault);
	mp_gui2->setVisible(true);
	mp_gui2->addWidgetDown(mp_ddlDevices);
 
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    float xGui3 = mp_gui2->getRect()->getLeft() + mp_gui2->getRect()->getWidth()+10;
    float wGui3 = ofGetWidth() - xGui3;
	mp_gui3 = new ofxUICanvas(xGui3,h,wGui3+10,300);
	mp_lblDeviceTitle = new ofxUILabel("Device", OFX_UI_FONT_LARGE);
    mp_gui3->addWidgetDown(mp_lblDeviceTitle);
	
	if (isSimulation)
	{
		mp_gui3->addWidgetDown( new ofxUIToggle(dim, dim, false, "mute sound input") );
	}
	
    
    mp_sliderDeviceVolMax = new ofxUISlider( wGui3-10, dim, 0.005f, 0.25f, 0.10f, "Vol. max" );
    mp_sliderDeviceVolHistorySize = new ofxUISlider( wGui3-10, dim, 50, 500, 400, "Vol. history size" );
    mp_toggleDeviceEnableStandby = new ofxUIToggle( dim, dim, true, "Enable standby");
    mp_sliderDeviceVolHistoryTh = new ofxUISlider( wGui3-10, dim, 0.0f, 0.75f, 0.5f, "Vol. history standby" );
    mp_sliderDeviceTimeStandby = new ofxUISlider( wGui3-10, dim, 5.0f, 20.0f, 10.0f, "Time standby" );
    mp_sliderDeviceNbLEDsStandby = new ofxUISlider( wGui3-10, dim, 10, 100, 50.0f, "Nb LEDs standby" );
    mp_sliderDeviceSpeedStandby = new ofxUISlider( wGui3-10, dim, 40, 360, 70.0f, "Speed standby" );
 
	mp_gui3->addWidgetDown(mp_sliderDeviceVolMax);
	mp_gui3->addWidgetDown(mp_sliderDeviceVolHistorySize);

    mp_gui3->addWidgetDown(new ofxUILabel("> Stand by", OFX_UI_FONT_SMALL));
    mp_gui3->addWidgetDown(mp_toggleDeviceEnableStandby);
	mp_gui3->addWidgetDown(mp_sliderDeviceVolHistoryTh);
	mp_gui3->addWidgetDown(mp_sliderDeviceTimeStandby);
	mp_gui3->addWidgetDown(mp_sliderDeviceNbLEDsStandby);
	mp_gui3->addWidgetDown(mp_sliderDeviceSpeedStandby);

    
    mp_gui3->autoSizeToFitWidgets();
    
    
    ofxUIRectangle* pGui3Rect = mp_gui3->getRect();
    mp_gui4 = new ofxUICanvas(pGui3Rect->getX(), pGui3Rect->getY()+pGui3Rect->getHeight() ,pGui3Rect->getWidth(),300);


 
    mp_gui4->addWidgetDown	( new ofxUILabel("SURFACE", OFX_UI_FONT_MEDIUM) );
    mp_gui4->addWidgetDown	( new ofxUISpacer(widthDefault, 2));

    mp_gui4->addWidgetDown	( new ofxUIToggle( dim, dim, false, "surf. enable standby") );
    mp_gui4->addWidgetRight	( new ofxUILabel("| state", OFX_UI_FONT_SMALL) );
	mp_lblSurfaceActivity = new ofxUILabel("1234", "4",OFX_UI_FONT_SMALL);
    mp_gui4->addWidgetRight ( mp_lblSurfaceActivity );
    mp_gui4->addWidgetDown(new ofxUISlider( wGui3-10, dim, 0.01f, 0.1f, 0.02f, "th. go standby" ));
    mp_gui4->addWidgetDown(new ofxUISlider( wGui3-10, dim, 0.01f, 0.1f, 0.02f, "th. go active" ));
    mp_gui4->addWidgetDown(new ofxUISlider( wGui3-10, dim, 10.0f, 60.0f, 20.0f, "dur. pre-standby" ));


    mp_gui4->addWidgetDown	( new ofxUILabel("Dimensions", OFX_UI_FONT_SMALL) );
    mp_gui4->addWidgetRight ( new ofxUITextInput("wSurface", "4" , 40, dim,0,0,OFX_UI_FONT_SMALL));
    mp_gui4->addWidgetRight	( new ofxUILabel("x", OFX_UI_FONT_SMALL) );
    mp_gui4->addWidgetRight	( new ofxUITextInput("hSurface", "3" , 40, dim,0,0,OFX_UI_FONT_SMALL));

    mp_gui4->addWidgetDown	( new ofxUILabel("FBO", OFX_UI_FONT_SMALL) );
    mp_gui4->addWidgetRight ( new ofxUITextInput("wFboSurface", "1000" , 40, dim,0,0,OFX_UI_FONT_SMALL));
    mp_gui4->addWidgetRight	( new ofxUILabel("x", OFX_UI_FONT_SMALL) );
    mp_gui4->addWidgetRight	( new ofxUITextInput("hFboSurface", "1000" , 40, dim,0,0,OFX_UI_FONT_SMALL));


    mp_gui4->addWidgetDown(new ofxUIToggle( dim, dim, false, "syphon"));
    mp_gui4->addWidgetRight(new ofxUIToggle( dim, dim, false, "target"));
    mp_gui4->addWidgetRight(new ofxUISlider( 100, dim, 1.0f, 8.0f, 2.0f, "target line w" ));
    mp_gui4->addWidgetDown(new ofxUIToggle( dim, dim, false, "launch madmapper @ start"));
    mp_gui4->addWidgetDown(new ofxUIToggle( dim, dim, false, "launch murmur @ rasp"));

    mp_gui4->addWidgetDown(new ofxUIToggle( dim, dim, isAnimationSequence, "activate sequence"));
	mp_gui4->addWidgetDown(new ofxUISlider( wGui3-10, dim, 0.5f, 2.0f, 0.75f, "transition duration (s)" ));
	mp_gui4->addWidgetDown(new ofxUISlider( wGui3-10, dim, 30.0f, 180.0f, 120.0f, "animation duration (s)" ));


    mp_lblAnimTitle = new ofxUILabel("> playing animation.js", OFX_UI_FONT_SMALL);
	mp_gui4->addWidgetDown(mp_lblAnimTitle);
	mp_gui4->addWidgetDown(new ofxUILabelButton( 100, false, "Reload",OFX_UI_FONT_SMALL));
	mp_gui4->addWidgetRight(new ofxUILabelButton( 100, false, "Edit",OFX_UI_FONT_SMALL));

    guiUpdateDeviceAnimationTitle();
    mp_gui4->setVisible(true);
	mp_gui4->autoSizeToFitWidgets();
	ofAddListener(mp_gui4->newGUIEvent, this, &testApp::guiEvent);
    mp_gui4->loadSettings("GUI/gui4_settings.xml");
    

    //--------------------------------------------------------------
    //--------------------------------------------------------------
	ofAddListener(mp_gui2->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_gui3->newGUIEvent, this, &testApp::guiEvent);
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    // TODO : iterate through the list of surfaces here
    if (mp_surfaceMain)
	{
        AnimationManager& animManager = mp_surfaceMain->getAnimationManager();
        vector<Animation*>& listAnimations = animManager.m_listAnimations;
        vector<Animation*>::iterator it = listAnimations.begin();
        for ( ; it != listAnimations.end() ; ++it)
        {
            Animation* pAnim = *it;

            ofxUIRectangle* pGui4Rect = mp_gui4->getRect();
            ofxUICanvas* pAnimCanvas = new ofxUICanvas(pGui4Rect->getX(), pGui4Rect->getY()+pGui4Rect->getHeight() ,pGui4Rect->getWidth(), 300);
            pAnim->setUICanvas( pAnimCanvas );
            pAnim->createUI();
            
            pAnimCanvas->autoSizeToFitWidgets();
            pAnimCanvas->setVisible(false);
            
            m_mapAnimationUI[pAnim] = pAnimCanvas;
        }
        
        mp_surfaceMain->loadAnimationsProperties();
    }
}

//--------------------------------------------------------------
void testApp::updateControls()
{
	if (mp_lblSurfaceActivity)
	{
	    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
		if (pSurfaceCurrent)
			mp_lblSurfaceActivity->setLabel(pSurfaceCurrent->getStateActivity() + " - " + ofToString( pSurfaceCurrent->getVolumePacketsHistoryMean() ));
	}
}

//--------------------------------------------------------------
void testApp::initJS()
{
	int err = ofxJSInitialize();
    if (err == 0)
        setupJS();
} 

//--------------------------------------------------------------
void testApp::update()
{
	// dt
	float dt = (float) ofGetLastFrameTime();

    // OSC
    m_oscReceiver.update();

    // Sound
    SoundManager::instance()->update();
    
    // Surfaces update & render offscreen
    if (mp_surfaceMain)
	{
        mp_surfaceMain->update(dt);
        mp_surfaceMain->renderOffscreen(isShowDevicePointSurfaces);
		mp_surfaceMain->publishSyphon();
    }
    
    // Devices
	if (isSimulation)
	{
	   vector<DeviceEchoSimulator*>::iterator it = m_listDeviceSimulator.begin();
	   for ( ; it != m_listDeviceSimulator.end() ; ++it)
	   {
		   DeviceEchoSimulator* pDeviceSim = *it;
		   if (pDeviceSim)
			   pDeviceSim->update(dt);
	   }
	}
 
    // Scenes
    if (mp_sceneVisualisation)
        mp_sceneVisualisation->update(dt);
	
	updateControls();
}


//--------------------------------------------------------------


//--------------------------------------------------------------
void testApp::draw()
{
    if (isViewSimulation)
    {
        // Draw Scene
        if (mp_sceneVisualisation)
            mp_sceneVisualisation->draw();

        // Draw sound input data
        Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
        if (pDeviceCurrent && mp_gui2->isVisible()){
 //           pDeviceCurrent->drawSoundInputVolume( 0.5f*(ofGetWidth()-pDeviceCurrent->getWidthSoundInputVolume()) ,ofGetHeight()-pDeviceCurrent->getHeightSoundInputVolume());
            pDeviceCurrent->drawSoundInputVolume( 0.5f*(ofGetWidth()) ,ofGetHeight()-pDeviceCurrent->getHeightSoundInputVolume());
        }
        
		if (m_isUpdateLayout)
		{
			guiUpdateListDevices(320);
		
			m_isUpdateLayout = false;
		}

     
/*        if (mp_gui1)
            mp_gui1->draw();
*/
    }
	else
    {
        if (mp_surfaceMain)
		{
			ofFbo& offSurface = mp_surfaceMain->getOffscreen();

			ofRectangle rectScreen(0,0,ofGetWidth(),ofGetHeight());
			m_rectSurfaceOff.set(0,0,offSurface.getWidth(),offSurface.getHeight());
			m_rectSurfaceOff.scaleTo(rectScreen);
			m_rectSurfaceOff.alignTo(rectScreen);


			ofClear(70,70,70);
            ofSetColor(255,255,255,255);
            mp_surfaceMain->getOffscreen().draw(m_rectSurfaceOff.getX(),m_rectSurfaceOff.getY(),m_rectSurfaceOff.getWidth(),m_rectSurfaceOff.getHeight());
//            mp_surfaceMain->drawCacheLEDs(m_diamCacheLEDs);
            if (isShowDevicePointSurfaces)
			{
                mp_surfaceMain->drawDevicePointSurface(m_rectSurfaceOff);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();

	if (name == "surf. enable standby")
	{
		if (pSurfaceCurrent)
			pSurfaceCurrent->setEnableStandby( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else
	if (name == "th. go standby")
	{
		if (pSurfaceCurrent)
			pSurfaceCurrent->setGoStandbyTh( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else
	if (name == "th. go active")
	{
		if (pSurfaceCurrent)
			pSurfaceCurrent->setGoActiveTh( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else if (name == "dur. pre-standby")
	{
		if (pSurfaceCurrent)
			pSurfaceCurrent->setDurationPreStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else
	if (name == "mute sound input")
	{
		if (pDeviceCurrent)
			pDeviceCurrent->mute( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else
	if (name == "launch madmapper @ start")
	{
		m_isLaunchMadMapper = ((ofxUIToggle *) e.widget)->getValue();
	}
	else
	if (name == "launch murmur @ rasp")
	{
		m_isLaunchMurmurRaspberry = ((ofxUIToggle *) e.widget)->getValue();
	}
	else
    if (name == "syphon")
	{
        if (pSurfaceCurrent)
            pSurfaceCurrent->setPublishSyphon( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else
    if (name == "target")
	{
        if (pSurfaceCurrent)
            pSurfaceCurrent->setRenderTarget( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else
	if (name == "target line w")
	{
        if (pSurfaceCurrent)
            pSurfaceCurrent->setRenderTargetLineWidth( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else
    if (name == "activate sequence")
    {
        if (pSurfaceCurrent)
            pSurfaceCurrent->setTimelineActive( ((ofxUIToggle *) e.widget)->getValue() );
    }
    else if (name == "animation duration (s)")
    {
        if (pSurfaceCurrent)
            pSurfaceCurrent->setDurationAnimation(  ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "transition duration (s)")
    {
        if (pSurfaceCurrent)
            pSurfaceCurrent->setDurationTransition(  ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "activate main sound")
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
    else if (name == "Port")
    {
        m_oscReceiverPort = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );
        printf("m_oscReceiverPort=%d\n", m_oscReceiverPort);
    }
    else if (name == "Reload")
    {
        if (pSurfaceCurrent)
            pSurfaceCurrent->getAnimationManager().M_reloadScript();
    }
    else if (name == "Edit")
    {
        if (pSurfaceCurrent)
            pSurfaceCurrent->getAnimationManager().M_editScript();
    }
    else if (name == "wSurface")
    {
        if (pSurfaceCurrent)
		{
			SurfaceNode * pSurfaceNodeCurrent = this->getSurfaceNode(pSurfaceCurrent);
			float wSurface = atof( ((ofxUITextInput *) e.widget)->getTextString().c_str() );
	    	pSurfaceNodeCurrent->setWidth(wSurface);
			pSurfaceNodeCurrent->setPosition(-0.5f*wSurface, pSurfaceNodeCurrent->getPosition().y, pSurfaceNodeCurrent->getPosition().z);
		
			onSurfaceModified(pSurfaceCurrent);
		}
	}
    else if (name == "hSurface")
    {
        if (pSurfaceCurrent)
		{
			SurfaceNode * pSurfaceNodeCurrent = this->getSurfaceNode(pSurfaceCurrent);
	    	pSurfaceNodeCurrent->setHeight( atof( ((ofxUITextInput *) e.widget)->getTextString().c_str() ) );
		
			onSurfaceModified(pSurfaceCurrent);
		}
	}
    else if (name == "wFboSurface")
    {
        if (pSurfaceCurrent)
		{
			int wPixels = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );;
			int hPixels = pSurfaceCurrent->getHeightPixels();
			pSurfaceCurrent->setDimensions(wPixels,hPixels);
		}
	}
    else if (name == "hFboSurface")
    {
        if (pSurfaceCurrent)
		{
			int wPixels = pSurfaceCurrent->getWidthPixels();
			int hPixels = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );;
			pSurfaceCurrent->setDimensions(wPixels,hPixels);
		}
	}
    else if (name == "View simulation")
    {
        isViewSimulation = ((ofxUIToggle *) e.widget)->getValue();
        showControls(isViewSimulation);
//        toggleView();
    }
    else if (name == "Show device points")
    {
        isShowDevicePointSurfaces = ((ofxUIToggle *) e.widget)->getValue();
    }
    else if (name == "cache LEDs diameter")
    {
        m_diamCacheLEDs = ((ofxUISlider *) e.widget)->getScaledValue();
    }
    else if (name == "Vol. max")
    {
        if (pDeviceCurrent)
            pDeviceCurrent->setSoundInputVolumeMax( ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "Vol. history size")
    {
        if (pDeviceCurrent)
            pDeviceCurrent->setSoundInputVolHistorySize( (int) ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "Enable standby")
    {
        if (pDeviceCurrent)
            pDeviceCurrent->setEnableStandbyMode( ((ofxUIToggle *) e.widget)->getValue() );
    }
    else if (name == "Vol. history standby")
    {
        if (pDeviceCurrent)
            pDeviceCurrent->setSoundInputVolHistoryTh( ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "Devices")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        if (selected.size()==1)
            selectDevice( selected[0]->getName() );
    }
    else if (name == "Show infos on projection")
    {
        m_isViewAnimProperties = ((ofxUIToggle *) e.widget)->getValue();
    }
    else if (name == "Time standby")
    {
        if (pDeviceCurrent){
            pDeviceCurrent->setTimeStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
        }
    }
    else if (name == "Nb LEDs standby")
    {
        if (pDeviceCurrent){
            pDeviceCurrent->setNbLEDsStandby( (int) ((ofxUISlider *) e.widget)->getScaledValue() );
        }
    }
    else if (name == "Speed standby")
    {
        if (pDeviceCurrent){
            pDeviceCurrent->setSpeedStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
        }
    }
    
}

//--------------------------------------------------------------
void testApp::selectDeviceWithIndex(int index)
{
    if (mp_deviceManager && index>=0 && index < mp_deviceManager->m_listDevices.size())
		selectDevice(mp_deviceManager->m_listDevices[index]->m_id);
}

//--------------------------------------------------------------
void testApp::selectDevice(string id)
{
    if (mp_deviceManager)
    {
        Device* pDevice = mp_deviceManager->setDeviceCurrent( id );
        if (pDevice)
        {
            // Reveal container
            if (mp_gui3) mp_gui3->setVisible(true);

            // Label of container
            if (mp_lblDeviceTitle) mp_lblDeviceTitle->setLabel(pDevice->m_id);

            // Animation
            guiUpdateDeviceAnimationTitle();
        
            // Data
            guiUpdateDevice(pDevice);
        }
    }
}

//--------------------------------------------------------------
void testApp::guiUpdateListDevices(int widthDefault)
{
    if (mp_deviceManager && mp_ddlDevices)
    {
//        mp_ddlDevices->clearToggles();
        vector<string> listDevicesId;
        mp_deviceManager->getDevicesListId(listDevicesId);

        printf("[testApp::guiUpdateListDevices]\n  - %d devices\n", (int)listDevicesId.size());
     
		mp_ddlDevices->setAutoClose(false);
        mp_ddlDevices->initToggles(listDevicesId, OFX_UI_FONT_SMALL);
    }
}

//--------------------------------------------------------------
void testApp::guiUpdateDeviceAnimationTitle()
{
    if (mp_lblAnimTitle){
        Surface* pSurfaceCurrent = this->getSurfaceForDeviceCurrent();
        if (pSurfaceCurrent && pSurfaceCurrent->getAnimationManager().mp_animationCurrent){
            mp_lblAnimTitle->setLabel("> surface '"+pSurfaceCurrent->getId()+"' / playing '"+pSurfaceCurrent->getAnimationManager().mp_animationCurrent->m_name+"'");
        }
    }
}

//--------------------------------------------------------------
void testApp::guiUpdateDevice(Device* pDevice)
{
    if (pDevice)
    {
        mp_sliderDeviceVolMax->setValue( pDevice->getSoundInputVolumeMax() );
        mp_sliderDeviceVolHistorySize->setValue( float(pDevice->getSoundInputVolHistorySize()) );
        mp_sliderDeviceVolHistoryTh->setValue( float(pDevice->getSoundInputVolHistoryTh()) );
        mp_toggleDeviceEnableStandby->setValue( pDevice->getEnableStandbyMode() );
        mp_sliderDeviceTimeStandby->setValue( pDevice->getTimeStandby() );
        mp_sliderDeviceNbLEDsStandby->setValue( pDevice->getNbLEDsStandby() );
        mp_sliderDeviceSpeedStandby->setValue( pDevice->getSpeedStandby() );
    }
}


//--------------------------------------------------------------
void testApp::guiUpdateViewSimulation()
{
    if (mp_tgViewSimu)
        mp_tgViewSimu->setValue( isViewSimulation );
}

//--------------------------------------------------------------
void testApp::guiShowAnimationPropsAll(bool is)
{
    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
    if (pSurfaceCurrent)
    {
        vector<Animation*>& listAnimations = pSurfaceCurrent->getAnimationManager().m_listAnimations;
        vector<Animation*>::iterator it = listAnimations.begin();
        for ( ; it!= listAnimations.end() ; ++it)
            (*it)->showUI(is);
    }
    
    mp_guiAnimProps = 0;
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels)
{
   vector<DeviceEchoSimulator*>::iterator it = m_listDeviceSimulator.begin();
   for ( ; it != m_listDeviceSimulator.end() ; ++it)
   {
	   DeviceEchoSimulator* pDeviceSim = *it;
	   if (pDeviceSim)
		   pDeviceSim->audioIn(input, bufferSize, nChannels);
   }
}

//--------------------------------------------------------------
void testApp::toggleView()
{
    isViewSimulation = !isViewSimulation;
    showControls(isViewSimulation);
    if (!isViewSimulation){
        if (!isShowDevicePointSurfaces)
            ofHideCursor();
    }
    else
        ofShowCursor();
}

//--------------------------------------------------------------
void testApp::showControls(bool is)
{
    if (mp_gui1) mp_gui1->setVisible(is);
    if (mp_gui2) mp_gui2->setVisible(is);
    if (mp_gui3) mp_gui3->setVisible(is);
    if (mp_gui4) mp_gui4->setVisible(is);
    if (mp_guiAnimProps) mp_guiAnimProps->setVisible(is);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == ' ')
    {
        toggleView();
        guiUpdateViewSimulation();
    }
	else if (key == OF_KEY_RIGHT)
    {
        Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
        if (pSurfaceCurrent)
        {
            // Goto next animation
            pSurfaceCurrent->getAnimationManager().gotoAnimationNext();

            // Update Title
            guiUpdateDeviceAnimationTitle();

            // Show animation interface
            guiShowAnimationPropsAll(false);
            if (!pSurfaceCurrent->isTimelineActive())
            {
                Animation* pAnimationCurrent = pSurfaceCurrent->getAnimationManager().mp_animationCurrent;
                if (pAnimationCurrent)
                    mp_guiAnimProps = pAnimationCurrent->showUI();
            }
        }
    }
	else if (key == OF_KEY_LEFT)
    {
        Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
        if (pSurfaceCurrent)
        {
            pSurfaceCurrent->getAnimationManager().gotoAnimationPrev();
            guiUpdateDeviceAnimationTitle();
        
            // Show animation interface
            guiShowAnimationPropsAll(false);
            if (!pSurfaceCurrent->isTimelineActive())
            {
                Animation* pAnimationCurrent = pSurfaceCurrent->getAnimationManager().mp_animationCurrent;
                if (pAnimationCurrent)
                    mp_guiAnimProps = pAnimationCurrent->showUI();
            }
        
        
        }
    }
	else if ( key >= '1' && key <='9')
	{
		Globals::instance()->mp_app->selectDeviceWithIndex((int)(key-'1'));
	}
    else
    {
        if (mp_sceneVisualisation)
            mp_sceneVisualisation->keyPressed(key);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    if (!isViewSimulation)
    {
    
    }
    else
    {
        if (m_isUserControls) return;
        
        if (mp_sceneVisualisation)
            mp_sceneVisualisation->mouseDragged(x, y);
    }
}


//--------------------------------------------------------------
void testApp::onSurfaceModified(Surface* pSurface)
{
	if(mp_sceneVisualisation && pSurface)
    {

   		vector<Device*>& listDevices = pSurface->getListDevices();
   		vector<Device*>::iterator it = listDevices.begin();
   		for ( ; it != listDevices.end() ; ++it)
   		{
			DeviceNode* pDeviceNode = mp_sceneVisualisation->getDeviceNode(*it);
            SurfaceNode* pSurfaceNode = mp_sceneVisualisation->getSurfaceNode(pSurface);
			if (pDeviceNode && pSurfaceNode)
            	pDeviceNode->setPositionNodeSurface( pSurfaceNode->getGlobalPositionDevicePointSurface(pDeviceNode->getDevice()) );
   		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    if (isViewSimulation)
    {
        m_isUserControls =
        mp_gui1->isHit(x,y) ||
        mp_gui2->isHit(x,y) ||
        mp_gui3->isHit(x,y) ||
        mp_gui4->isHit(x,y) ||
        (mp_guiAnimProps && mp_guiAnimProps->isHit(x,y));

        if (m_isUserControls) return;
        
        if (mp_sceneVisualisation)
            mp_sceneVisualisation->mousePressed(x, y);
    }
    else
    {
        if (mp_guiAnimProps && mp_guiAnimProps->isHit(x,y))
            return;
     
        Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
        
        if (pDeviceCurrent)
        {
            Surface* pSurfaceCurrent = getSurfaceForDevice(pDeviceCurrent);
            if (pSurfaceCurrent)
            {
				float dx = x - m_rectSurfaceOff.getX();
				float dy = y - m_rectSurfaceOff.getY();
				
				  

				float xNorm = ofClamp(dx / m_rectSurfaceOff.getWidth(),0.0f,1.0f);
				float yNorm = ofClamp(dy / m_rectSurfaceOff.getHeight(),0.0f,1.0f);

			
                pDeviceCurrent->setPointSurface(xNorm, yNorm);
//                pDeviceCurrent->setPointSurface(float(x)/pSurfaceCurrent->getWidthPixels(), float(y)/pSurfaceCurrent->getHeightPixels());
            
            
                 //printf("%.3f,%.3f\n",float(x)/pSurfaceCurrent->getWidthPixels(),float(y)/pSurfaceCurrent->getHeightPixels());
                
                // Update simulation
                if(mp_sceneVisualisation)
                {
                    DeviceNode* pDeviceNode = mp_sceneVisualisation->getDeviceNode(pDeviceCurrent);
                    SurfaceNode* pSurfaceNode = mp_sceneVisualisation->getSurfaceNode(pSurfaceCurrent);
                    if (pDeviceNode && pSurfaceNode)
                        pDeviceNode->setPositionNodeSurface( pSurfaceNode->getGlobalPositionDevicePointSurface(pDeviceNode->getDevice()) );
                    
                }
            }
        }
    }
 
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	if (isViewSimulation)
    {
        if (!m_isUserControls)
        {
            if (mp_sceneVisualisation)
                mp_sceneVisualisation->mouseReleased(x, y);
        }
        m_isUserControls = false;
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

