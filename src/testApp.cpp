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
	
	mp_guiMain = 0;
	mp_guiNetwork = 0;
	mp_guiSound = 0;
	mp_gui1 = 0;
    mp_gui2 = 0;
    mp_gui3 = 0;
    mp_gui4 = 0;
	mp_guiAnimations = 0;
    mp_guiAnimProps = 0;
	mp_radioPanels = 0;
    mp_ddlDevices = 0;
    mp_lblAnimTitle = 0;
	mp_lblAnimDirJs = 0;
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
	mp_consoleJs = 0;
	m_listLogJsMax = 20;
    
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
	m_isLaunchDevices = false;
    
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
	if (m_isLaunchDevices && !isSimulation)
	{
		launchDevices();
	}
	
	// GO
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
    vidRecorder.setVideoCodec("mpeg4"); 
    vidRecorder.setVideoBitrate("800k");
//    vidRecorder.setAudioCodec("mp3");
//    vidRecorder.setAudioBitrate("192k");
	vidRecorder.setFfmpegLocation( ofFilePath::getAbsolutePath("Video/ffmpeg", true) );
	// ofLog() << ofFilePath::getAbsolutePath("Video/ffmpeg", true);

//	vidRecorder.setup("test.mov", ofGetWidth(), ofGetHeight(), 30);
}


//--------------------------------------------------------------
/*void testApp::videoSaved(ofVideoSavedEventArgs& e){
	// the ofQTKitGrabber sends a message with the file name and any errors when the video is done recording
	if(e.error.empty())
	{
		ofSystem("open " + e.videoPath);
	}
	else {
	   	ofLogError("videoSavedEvent") << "Video save error: " << e.error;
	}
}*/


//--------------------------------------------------------------
void testApp::launchDevices()
{
	printf("[testApp::launchDevices]\n");
	int nbLaunchDevices = m_settings.getNumTags("murmur:raspberry:ip");
	for (int i=0;i<nbLaunchDevices;i++)
	{
		string strIPMurmur = m_settings.getValue("murmur:raspberry:ip", "10.23.108.114", i);
		string strRun = "ssh pi@" + strIPMurmur + " /home/pi/Dev/C/openFrameworks/examples/myapps/murmurRaspberry/bin/run_murmur.sh";
		threadRasp* pThreadLaunchDevice = new threadRasp();
		pThreadLaunchDevice->setCommand(strRun);
		pThreadLaunchDevice->startThread();

		printf("  - running \"%s\"\n", strRun.c_str());

		m_listThreadLaunchDevices.push_back(pThreadLaunchDevice);
	}
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

	if (mp_guiMain)
		mp_guiMain->saveSettings("GUI/guiMain_settings.xml");
 
	if (mp_guiNetwork)
		 mp_guiNetwork->saveSettings("GUI/guiNetwork_settings.xml");

	if (mp_guiAnimations)
		 mp_guiAnimations->saveSettings("GUI/guiAnimations_settings.xml");
	
	if (mp_guiSound)
		 mp_guiSound->saveSettings("GUI/guiSound_settings.xml");
 
	if (mp_gui1)
        mp_gui1->saveSettings("GUI/gui1_settings.xml");
 
	if (mp_gui4)
        mp_gui4->saveSettings("GUI/gui4_settings.xml");
	
 
    delete mp_sceneVisualisation;
    delete mp_deviceManager;
    delete mp_deviceInfoManager;
    delete Globals::instance();
    delete Data::instance();
	delete mp_guiMain;
	delete mp_guiNetwork;
	delete mp_guiSound;
	delete mp_guiAnimations;
	delete mp_gui1;
	delete mp_gui2;
	delete mp_gui3;
	delete mp_gui4;
	
	vector<threadRasp*>::iterator it;
	for (it = m_listThreadLaunchDevices.begin(); it!=m_listThreadLaunchDevices.end(); ++it)
	{
		(*it)->waitForThread(true);
		delete (*it);
	}

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

	// TODO : where to put this
	// TODO : also when updating the fbo
#if MURMUR_WARPING
    m_surfaceQuadWarp.setSourceRect( ofRectangle( 0, 0, mp_surfaceMain->getOffscreen().getWidth(), mp_surfaceMain->getOffscreen().getHeight() ) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    m_surfaceQuadWarp.setTopLeftCornerPosition( ofPoint( 0, 0 ) );             // this is position of the quad warp corners, centering the image on the screen.
    m_surfaceQuadWarp.setTopRightCornerPosition( ofPoint( ofGetWidth(), 0 ) );        // this is position of the quad warp corners, centering the image on the screen.
    m_surfaceQuadWarp.setBottomLeftCornerPosition( ofPoint( 0 ,  ofGetHeight() ) );      // this is position of the quad warp corners, centering the image on the screen.
    m_surfaceQuadWarp.setBottomRightCornerPosition( ofPoint( ofGetWidth(), ofGetHeight() ) ); // this is position of the quad warp corners, centering the image on the screen.
    m_surfaceQuadWarp.setup();
#endif
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
	float h = 0;
	ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
    
    //--------------------------------------------------------------
	// Main panel info
    //--------------------------------------------------------------
	vector<string> panelNames;
	panelNames.push_back("Configuration");
	panelNames.push_back("Network");
	panelNames.push_back("Devices");
	panelNames.push_back("Animations");
	panelNames.push_back("Surfaces");
	panelNames.push_back("Scene");
	panelNames.push_back("Sound");

	mp_guiMain = new ofxUICanvas(0,0,widthDefault,1200);
	mp_guiMain->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    mp_guiMain->addWidgetDown(new ofxUIImage(0,0,238,60,&Data::instance()->m_imgLogo,"logo",false));
    mp_guiMain->addWidgetDown(new ofxUIFPS(fontType));
	mp_radioPanels = mp_guiMain->addRadio("Panels", panelNames, OFX_UI_ORIENTATION_VERTICAL, OFX_UI_FONT_MEDIUM);
	
	mp_guiMain->autoSizeToFitWidgets();

	float xPanel = mp_guiMain->getRect()->getWidth()+10;

    //--------------------------------------------------------------
	// Configuration
    //--------------------------------------------------------------
	mp_gui1 = new ofxUICanvas(xPanel,h,widthDefault,1200);
	mp_gui1->setName("Configuration");
	mp_gui1->addWidgetDown(new ofxUILabel("Configuration", OFX_UI_FONT_LARGE));
    mp_gui1->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_gui1->addToggle("Show device points", isShowDevicePointSurfaces, dim, dim);
    mp_tgViewSimu = mp_gui1->addToggle("View simulation", isViewSimulation, dim, dim);

    mp_gui1->addWidgetDown(new ofxUIToggle( "launch madmapper @ start", false, dim, dim));
    mp_gui1->addWidgetDown(new ofxUIToggle("launch murmur @ rasp", false, dim, dim));

	mp_gui1->autoSizeToFitWidgets();
	mp_gui1->setVisible(false);

	m_listPanels.push_back(mp_gui1);

    //--------------------------------------------------------------
	// Network
    //--------------------------------------------------------------
	mp_guiNetwork = new ofxUICanvas(xPanel,h,widthDefault,1200);
    mp_guiNetwork->addWidgetDown(new ofxUILabel("Network", OFX_UI_FONT_LARGE));
    mp_guiNetwork->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_guiNetwork->addWidgetDown(new ofxUILabel("port : ", fontType));
    mp_guiNetwork->addWidgetRight(new ofxUITextInput("Port", ofToString(m_oscReceiverPort) , 100, dim,0,0,fontType));

	mp_guiNetwork->autoSizeToFitWidgets();
	mp_guiNetwork->setVisible(false);

	m_listPanels.push_back(mp_guiNetwork);

    //--------------------------------------------------------------
	// Devices
    //--------------------------------------------------------------
	mp_gui2 = new ofxUICanvas(xPanel,h,widthDefault,200);
//	mp_gui2->setName("Devices");
    vector<string> listDevicesIds;
	mp_ddlDevices = new ofxUIDropDownList("", listDevicesIds);
    guiUpdateListDevices(widthDefault);
	mp_gui2->setVisible(true);
	mp_gui2->addWidgetDown(mp_ddlDevices);

//	mp_gui2->autoSizeToFitWidgets();
	mp_gui2->setVisible(false);
	
	m_listPanels.push_back(mp_gui2);
	
    //--------------------------------------------------------------
    float xGui3 = xPanel+widthDefault+10;
    float wGui3 = widthDefault;
	mp_gui3 = new ofxUICanvas(xGui3,h,wGui3+10,300);
	mp_gui3->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	mp_lblDeviceTitle = new ofxUILabel("Devices", OFX_UI_FONT_LARGE);
    mp_gui3->addWidgetDown(mp_lblDeviceTitle);
    mp_gui3->addWidgetDown(new ofxUISpacer(widthDefault, 2));
	
	if (isSimulation)
	{
		mp_gui3->addWidgetDown( new ofxUIToggle("mute sound input", false, dim, dim) );
	}
 
    mp_sliderDeviceVolMax = new ofxUISlider("Vol. max", 0.005f, 0.04f, 0.02f, wGui3-10, dim );
    mp_sliderDeviceVolHistorySize = new ofxUISlider("Vol. history size", 50, 500, 400, wGui3-10, dim );
    mp_toggleDeviceEnableStandby = new ofxUIToggle("Enable standby", true, dim, dim);
    mp_sliderDeviceVolHistoryTh = new ofxUISlider( "Vol. history standby", 0.0f, 0.75f, 0.5f, wGui3-10, dim );
    mp_sliderDeviceTimeStandby = new ofxUISlider("Time standby", 5.0f, 20.0f, 10.0f, wGui3-10, dim );
    mp_sliderDeviceNbLEDsStandby = new ofxUISlider( "Nb LEDs standby", 10, 100, 50.0f, wGui3-10, dim );
    mp_sliderDeviceSpeedStandby = new ofxUISlider( "Speed standby", 40, 360, 70.0f, wGui3-10, dim );
 
	mp_gui3->addWidgetDown(mp_sliderDeviceVolMax);
	mp_gui3->addWidgetDown(mp_sliderDeviceVolHistorySize);

    mp_gui3->addWidgetDown(new ofxUILabel("> Stand by", fontType));
    mp_gui3->addWidgetDown(mp_toggleDeviceEnableStandby);
	mp_gui3->addWidgetDown(mp_sliderDeviceVolHistoryTh);
	mp_gui3->addWidgetDown(mp_sliderDeviceTimeStandby);
	mp_gui3->addWidgetDown(mp_sliderDeviceNbLEDsStandby);
	mp_gui3->addWidgetDown(mp_sliderDeviceSpeedStandby);

	mp_gui3->autoSizeToFitWidgets();
	mp_gui3->setVisible(false);

	m_listPanels.push_back(mp_gui3);


    //--------------------------------------------------------------
	// Animations
    //--------------------------------------------------------------
	mp_guiAnimations = new ofxUICanvas(xPanel,h,widthDefault,300);
    mp_guiAnimations->addWidgetDown(new ofxUILabel("Animations", OFX_UI_FONT_LARGE));
    mp_guiAnimations->addWidgetDown(new ofxUISpacer(widthDefault, 2));
	mp_guiAnimations->addWidgetDown(new ofxUIToggle("activate sequence", isAnimationSequence, dim, dim));
	mp_guiAnimations->addWidgetDown(new ofxUISlider("transition duration (s)", 0.5f, 2.0f, 0.75f, wGui3-10, dim ));
	mp_guiAnimations->addWidgetDown(new ofxUISlider("animation duration (s)", 30.0f, 180.0f, 120.0f, wGui3-10, dim ));

	mp_guiAnimations->addWidgetDown(new ofxUILabel("Scripts", OFX_UI_FONT_MEDIUM));
    mp_guiAnimations->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_lblAnimDirJs = new ofxUILabel("> dir", fontType);
    mp_lblAnimTitle = new ofxUILabel("> playing animation.js", fontType);
	mp_guiAnimations->addWidgetDown(mp_lblAnimDirJs);
	mp_guiAnimations->addWidgetDown(mp_lblAnimTitle);
	mp_guiAnimations->addWidgetDown(new ofxUILabelButton("Reload", 100, false,fontType));
	mp_guiAnimations->addWidgetRight(new ofxUILabelButton("Edit", 100, false,fontType));
	mp_guiAnimations->addWidgetDown(new ofxUILabel("Console", OFX_UI_FONT_MEDIUM));
    mp_guiAnimations->addWidgetDown(new ofxUISpacer(widthDefault, 2));
	mp_consoleJs = new ofxUITextArea("Console", "", widthDefault, 300,0,0,fontType);
	mp_guiAnimations->addWidgetDown(mp_consoleJs);


//void ofxUITextArea::init(string _name, string _textstring, float w, float h, float x, float y, int _size)


    mp_guiAnimations->setVisible(false);
	mp_guiAnimations->autoSizeToFitWidgets();

    guiUpdateDeviceAnimationTitle();


	m_listPanels.push_back(mp_guiAnimations);

    //--------------------------------------------------------------
	// Surfaces
    //--------------------------------------------------------------
    mp_gui4 = new ofxUICanvas(xPanel, h , widthDefault,300);
	mp_gui4->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
 
    mp_gui4->addWidgetDown	( new ofxUILabel("Surfaces", OFX_UI_FONT_MEDIUM) );
    mp_gui4->addWidgetDown	( new ofxUISpacer(widthDefault, 2));

    mp_gui4->addWidgetDown	( new ofxUIToggle("surf. enable standby", false, dim, dim) );
    mp_gui4->addWidgetRight	( new ofxUILabel("| state", fontType) );
	mp_lblSurfaceActivity = new ofxUILabel("1234", "4",fontType);
    mp_gui4->addWidgetRight ( mp_lblSurfaceActivity );
    mp_gui4->addWidgetDown(new ofxUISlider( "th. go standby", 0.01f, 0.1f, 0.02f,wGui3-10, dim ));
    mp_gui4->addWidgetDown(new ofxUISlider( "th. go active", 0.01f, 0.1f, 0.02f,wGui3-10, dim ));
    mp_gui4->addWidgetDown(new ofxUISlider( "dur. pre-standby", 10.0f, 60.0f, 20.0f,wGui3-10, dim ));


    mp_gui4->addWidgetDown	( new ofxUILabel("Dimensions", fontType) );
    mp_gui4->addWidgetRight ( new ofxUITextInput("wSurface", "4" , 40, dim,0,0,fontType));
    mp_gui4->addWidgetRight	( new ofxUILabel("x", fontType) );
    mp_gui4->addWidgetRight	( new ofxUITextInput("hSurface", "3" , 40, dim,0,0,fontType));

    mp_gui4->addWidgetDown	( new ofxUILabel("FBO", fontType) );
    mp_gui4->addWidgetRight ( new ofxUITextInput("wFboSurface", "1000" , 40, dim,0,0,fontType));
    mp_gui4->addWidgetRight	( new ofxUILabel("x", fontType) );
    mp_gui4->addWidgetRight	( new ofxUITextInput("hFboSurface", "1000" , 40, dim,0,0,fontType));


    mp_gui4->addWidgetDown(new ofxUIToggle("syphon", false, dim, dim));
    mp_gui4->addWidgetRight(new ofxUIToggle("target", false, dim, dim));
    mp_gui4->addWidgetRight(new ofxUISlider("target line w", 1.0f, 8.0f, 2.0f, 100, dim ));


	mp_gui4->setVisible(false);
	mp_gui4->autoSizeToFitWidgets();


	m_listPanels.push_back(mp_gui4);

    //--------------------------------------------------------------
	// Sound
    //--------------------------------------------------------------
	mp_guiSound = new ofxUICanvas(xPanel,h,widthDefault,1200);

    mp_guiSound->addWidgetDown(new ofxUILabel("Sounds", OFX_UI_FONT_LARGE));
    mp_guiSound->addWidgetDown(new ofxUISpacer(widthDefault, 2));
    mp_guiSound->addWidgetDown(new ofxUIToggle( "activate main sound", isAnimationSequence, dim, dim));
	mp_guiSound->addWidgetDown(new ofxUISlider( "vol. main sound min.", 0.0f, 1.0f, 0.5f, widthDefault-10, dim));
	mp_guiSound->addWidgetDown(new ofxUISlider( "vol. main sound max.", 0.0f, 1.0f, 1.0f, widthDefault-10, dim));

//    mp_guiSound->addWidgetDown(new ofxUILabel("Library", OFX_UI_FONT_LARGE));
//    mp_guiSound->addWidgetDown(new ofxUISpacer(widthDefault, 2));

	ofxUIDropDownList* mp_ddlSounds = new ofxUIDropDownList("Library", SoundManager::instance()->getListSoundsName());
//	mp_ddlSounds->setLabelText("Sounds");
	mp_ddlSounds->open();
	mp_guiSound->addWidgetDown(mp_ddlSounds);

	mp_guiSound->autoSizeToFitWidgets();
	mp_guiSound->setVisible(false);

	m_listPanels.push_back(mp_guiSound);

    //--------------------------------------------------------------
    //--------------------------------------------------------------
	ofAddListener(mp_guiMain->newGUIEvent, this, &testApp::guiMainEvent);
	ofAddListener(mp_guiNetwork->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_guiAnimations->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_gui1->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_gui2->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_gui3->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_gui4->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(mp_guiSound->newGUIEvent, this, &testApp::guiEvent);
	
	 mp_gui1->loadSettings("GUI/gui1_settings.xml");
	 mp_gui4->loadSettings("GUI/gui4_settings.xml");
	 mp_guiNetwork->loadSettings("GUI/guiNetwork_settings.xml");
	 mp_guiSound->loadSettings("GUI/guiSound_settings.xml");
	 mp_guiAnimations->loadSettings("GUI/guiAnimations_settings.xml");
 
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

            ofxUIRectangle* pGuiAnimRect = mp_guiAnimations->getRect();
            ofxUICanvas* pAnimCanvas = new ofxUICanvas(pGuiAnimRect->getX()+pGuiAnimRect->getWidth()+10, pGuiAnimRect->getY(), widthDefault, 300);
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
	{
		// Log
		ofxJSPrintCallback((void*)this, logJS);

		// Create some new functions
        setupJS();
	}
}

//--------------------------------------------------------------
void testApp::logJS(void* pData, const string& message)
{
   std::cout << message;
   testApp* pThis = (testApp*) pData;
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
        if (pDeviceCurrent && mp_gui2 && mp_gui2->isVisible())
		{
            pDeviceCurrent->drawSoundInputVolume( 0.5f*(ofGetWidth()) ,ofGetHeight()-pDeviceCurrent->getHeightSoundInputVolume());
        }
        
		if (m_isUpdateLayout)
		{
			guiUpdateListDevices(320);
			m_isUpdateLayout = false;
		}

		recordingImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        vidRecorder.addFrame(recordingImage.getPixelsRef());
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
			
			#if MURMUR_WARPING
				ofMatrix4x4 mat = m_surfaceQuadWarp.getMatrix();
				
				//======================== use the matrix to transform our fbo.
				
				glPushMatrix();
				glMultMatrixf( mat.getPtr() );
				{
					mp_surfaceMain->getOffscreen().draw( 0, 0 );
				}
				glPopMatrix();
			#endif

        }
    }
}

//--------------------------------------------------------------
void testApp::guiMainEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
	if (name == "Panels")
	{
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        //cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl;
		vector<ofxUICanvas*>::iterator it;
		for (it = m_listPanels.begin() ; it!=m_listPanels.end(); ++it){
			(*it)->setVisible(false);
		}
		if (mp_guiAnimProps)
			mp_guiAnimProps->setVisible(false);

		string panelName = radio->getActiveName();
		
		if (panelName == "Configuration"){
			if (mp_gui1) mp_gui1->setVisible(true);
		}
		else if (panelName == "Network"){
			if (mp_guiNetwork) mp_guiNetwork->setVisible(true);
		}
		else if (panelName == "Devices"){
			if (mp_gui2) mp_gui2->setVisible(true);
			if (mp_gui3) mp_gui3->setVisible(true);
		}
		else if (panelName == "Animations"){
			if (mp_guiAnimations) mp_guiAnimations->setVisible(true);
			if (mp_guiAnimProps) mp_guiAnimProps->setVisible(true);
		}
		else if (panelName == "Surfaces"){
			if (mp_gui4) mp_gui4->setVisible(true);
		}
		else if (panelName == "Sound"){
			if (mp_guiSound) mp_guiSound->setVisible(true);
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
		m_isLaunchDevices = ((ofxUIToggle *) e.widget)->getValue();
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
    else if (name == "Library")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        if (selected.size()==1){
			//printf("selected %s", selected[0]->getName().c_str());
	    	
		}
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
       // showControls(isViewSimulation);
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
			if (mp_radioPanels) mp_radioPanels->activateToggle("Devices");
	
//            if (mp_gui3) mp_gui3->setVisible(true);

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
        mp_ddlDevices->init("Devices",listDevicesId);
    }
}

//--------------------------------------------------------------
void testApp::guiUpdateDeviceAnimationTitle()
{
   Surface* pSurfaceCurrent = this->getSurfaceForDeviceCurrent();
   if (pSurfaceCurrent)
   {
   	 if (mp_lblAnimDirJs)
	 {
			 mp_lblAnimDirJs->setLabel("> dir "+pSurfaceCurrent->m_strDirScripts);
	 }
   
	 if (mp_lblAnimTitle)
	 {
	 	if (pSurfaceCurrent->getAnimationManager().mp_animationCurrent)
	 		mp_lblAnimTitle->setLabel("> surface '"+pSurfaceCurrent->getId()+"' / playing '"+pSurfaceCurrent->getAnimationManager().mp_animationCurrent->m_name+"'");
	 }
	}
}

//--------------------------------------------------------------
void testApp::guiUpdateDevice(Device* pDevice)
{
    if (pDevice)
    {
        if (mp_sliderDeviceVolMax) mp_sliderDeviceVolMax->setValue( pDevice->getSoundInputVolumeMax() );
        if (mp_sliderDeviceVolHistorySize) mp_sliderDeviceVolHistorySize->setValue( float(pDevice->getSoundInputVolHistorySize()) );
        if (mp_sliderDeviceVolHistoryTh) mp_sliderDeviceVolHistoryTh->setValue( float(pDevice->getSoundInputVolHistoryTh()) );
        if (mp_toggleDeviceEnableStandby) mp_toggleDeviceEnableStandby->setValue( pDevice->getEnableStandbyMode() );
        if (mp_sliderDeviceTimeStandby) mp_sliderDeviceTimeStandby->setValue( pDevice->getTimeStandby() );
        if (mp_sliderDeviceNbLEDsStandby) mp_sliderDeviceNbLEDsStandby->setValue( pDevice->getNbLEDsStandby() );
        if (mp_sliderDeviceSpeedStandby) mp_sliderDeviceSpeedStandby->setValue( pDevice->getSpeedStandby() );
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
void testApp::saveWidgetState(ofxUIWidget* pWidget)
{
	if (pWidget)
		m_mapControlsState[pWidget] = pWidget->isVisible();
}

//--------------------------------------------------------------
void testApp::saveControlsState()
{
	saveWidgetState(mp_guiMain);
	saveWidgetState(mp_gui1);
	saveWidgetState(mp_gui2);
	saveWidgetState(mp_gui3);
	saveWidgetState(mp_gui4);
	saveWidgetState(mp_guiSound);
	saveWidgetState(mp_guiNetwork);
	saveWidgetState(mp_guiAnimations);
	saveWidgetState(mp_guiAnimProps);
}

//--------------------------------------------------------------
void testApp::restoreControlsState()
{
	if (mp_guiMain) mp_guiMain->setVisible(m_mapControlsState[mp_guiMain]);
    if (mp_gui1) mp_gui1->setVisible(m_mapControlsState[mp_gui1]);
    if (mp_gui2) mp_gui2->setVisible(m_mapControlsState[mp_gui2]);
    if (mp_gui3) mp_gui3->setVisible(m_mapControlsState[mp_gui3]);
    if (mp_gui4) mp_gui4->setVisible(m_mapControlsState[mp_gui4]);
    if (mp_guiSound) mp_guiSound->setVisible(m_mapControlsState[mp_guiSound]);
    if (mp_guiNetwork) mp_guiNetwork->setVisible(m_mapControlsState[mp_guiNetwork]);
    if (mp_guiAnimations) mp_guiAnimations->setVisible(m_mapControlsState[mp_guiAnimations]);
    if (mp_guiAnimProps) mp_guiAnimProps->setVisible(m_mapControlsState[mp_guiAnimProps]);
}

//--------------------------------------------------------------
void testApp::showControls(bool is)
{
	if (is)
		restoreControlsState();
	else{
		saveControlsState();
		if (mp_guiMain && mp_guiMain->isVisible()) mp_guiMain->setVisible(false);
    	if (mp_gui1 && mp_gui1->isVisible()) mp_gui1->setVisible(false);
    	if (mp_gui2 && mp_gui2->isVisible()) mp_gui2->setVisible(false);
    	if (mp_gui3 && mp_gui3->isVisible()) mp_gui3->setVisible(false);
    	if (mp_gui4 && mp_gui4->isVisible()) mp_gui4->setVisible(false);
    	if (mp_guiSound && mp_guiSound->isVisible()) mp_guiSound->setVisible(false);
    	if (mp_guiNetwork && mp_guiNetwork->isVisible()) mp_guiNetwork->setVisible(false);
    	if (mp_guiAnimations && mp_guiAnimations->isVisible()) mp_guiAnimations->setVisible(false);
    	if (mp_guiAnimProps && mp_guiAnimProps->isVisible()) mp_guiAnimProps->setVisible(false);
	}
/*
	if (mp_guiMain) mp_guiMain->setVisible(is);
    if (mp_gui1) mp_gui1->setVisible(is);
    if (mp_gui2) mp_gui2->setVisible(is);
    if (mp_gui3) mp_gui3->setVisible(is);
    if (mp_gui4) mp_gui4->setVisible(is);
    if (mp_guiSound) mp_guiSound->setVisible(is);
    if (mp_guiNetwork) mp_guiNetwork->setVisible(is);
    if (mp_guiAnimations) mp_guiAnimations->setVisible(is);
    if (mp_guiAnimProps) mp_guiAnimProps->setVisible(is);
*/

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == ' ')
    {
        toggleView();
        guiUpdateViewSimulation();
    }
	else
	if (key == OF_KEY_RIGHT)
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
                    mp_guiAnimProps = pAnimationCurrent->getUI();
				if (mp_radioPanels && mp_radioPanels->getActiveName() == "Animations" && mp_guiAnimProps){
					mp_guiAnimProps->setVisible(true);
				}
            }
        }
    }
	else
	if (key == OF_KEY_LEFT)
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
                    mp_guiAnimProps = pAnimationCurrent->getUI();
				if (mp_radioPanels && mp_radioPanels->getActiveName() == "Animations" && mp_guiAnimProps){
					mp_guiAnimProps->setVisible(true);
				}
            }
        }
    }
	else
	if ( key >= '1' && key <='9')
	{
		Globals::instance()->mp_app->selectDeviceWithIndex((int)(key-'1'));
	}
	else
	if (key == 'x')
	{
		vidRecorder.close();
		ofLog() << vidRecorder.getMoviePath();
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
    if (isViewSimulation && mp_gui1 && mp_gui2 && mp_gui3 && mp_gui4)
    {
        m_isUserControls =
        mp_gui1->isHit(x,y) ||
        mp_gui2->isHit(x,y) ||
        mp_gui3->isHit(x,y) ||
        mp_gui4->isHit(x,y) ||
        mp_guiNetwork->isHit(x,y) ||
        mp_guiAnimations->isHit(x,y) ||
        mp_guiMain->isHit(x,y) ||
        mp_guiSound->isHit(x,y) ||
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

