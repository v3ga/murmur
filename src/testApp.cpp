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
    if (isSimulation && mp_deviceSimulator)
    {
        int deviceSoundInputId = m_settings.getValue("murmur:simulator:soundInput:device", -1);
        int nbChannels = m_settings.getValue("murmur:simulator:soundInput:nbChannels", 2);
        printf("deviceSoundInputId=%d\n",deviceSoundInputId);
        printf("nbChannels=%d\n",nbChannels);

        mp_deviceSimulator->startSoundInput(deviceSoundInputId,nbChannels);
        mp_deviceSimulator->loadXML("Config/");
        if (mp_deviceNodeSim)
            mp_deviceNodeSim->setPositionNodeSurface( mp_surfaceNodeMain->getGlobalPositionDevicePointSurface( mp_deviceSimulator ) );
        selectDevice("deviceEchoSimulator");

    
    }

	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::exit()
{
    if (mp_deviceManager)
        mp_deviceManager->saveDevicesXML("Config/");

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
}

//--------------------------------------------------------------
void testApp::initDevices()
{
    if (isSimulation)
    {
        int nbLEDs = m_settings.getValue("murmur:simulator:device:nbLEDs", 129);
        float length = m_settings.getValue("murmur:simulator:device:length", 4.0f);

        
        mp_deviceSimulator = new DeviceEchoSimulator("deviceEchoSimulator", nbLEDs, length/float(nbLEDs-1));
    
        mp_deviceManager->addDevice(mp_deviceSimulator);
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
    if (mp_surfaceMain){
        if (isSimulation){
            mp_surfaceMain->addDevice(mp_deviceSimulator);
        }
    }
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
    
    // Devices nodes
    if (isSimulation)
        mp_deviceNodeSim = new DeviceNode( mp_deviceSimulator );
    
    // Construct the scene
    mp_sceneVisualisation = new SceneVisualisation();
    mp_sceneVisualisation->setup();
    mp_sceneVisualisation->addSurfaceNode(mp_surfaceNodeMain);

    if (isSimulation)
        mp_sceneVisualisation->addDeviceNode(mp_deviceNodeSim, mp_surfaceNodeMain);
    
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
	mp_gui2 = new ofxUICanvas(mp_gui1->getRect()->getWidth()+10,h,widthDefault+10,200);
    vector<string> listDeviceIds;
	mp_ddlDevices = new ofxUIDropDownList(widthDefault, "Devices", listDeviceIds, OFX_UI_FONT_LARGE);
	mp_ddlDevices->setAutoClose(false);
    guiUpdateListDevices();
    mp_gui2->addWidgetDown(mp_ddlDevices);
    mp_gui2->setVisible(true);
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    float xGui3 = mp_gui2->getRect()->getLeft() + mp_gui2->getRect()->getWidth()+10;
    float wGui3 = ofGetWidth() - xGui3;
	mp_gui3 = new ofxUICanvas(xGui3,h,wGui3+10,300);
	mp_lblDeviceTitle = new ofxUILabel("Device", OFX_UI_FONT_LARGE);
    mp_gui3->addWidgetDown(mp_lblDeviceTitle);
    
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
    
    mp_gui4->addWidgetDown( new ofxUILabel("SURFACE", OFX_UI_FONT_MEDIUM) );
    mp_gui4->addWidgetDown(new ofxUISpacer(widthDefault, 2));
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
    if (mp_surfaceMain){
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
    if (mp_surfaceMain){
        mp_surfaceMain->update(dt);
        mp_surfaceMain->renderOffscreen();
    }
    
    // Devices
    if (mp_deviceSimulator){
        mp_deviceSimulator->update(dt);
    }
    
    // Scenes
    if (mp_sceneVisualisation)
        mp_sceneVisualisation->update(dt);
}

//--------------------------------------------------------------
void testApp::draw()
{
    if (!isViewSimulation)
    {
        if (mp_surfaceMain){
            ofSetColor(255,255,255,255);
            mp_surfaceMain->getOffscreen().draw(0,0,ofGetWidth(),ofGetHeight());
            mp_surfaceMain->drawCacheLEDs(m_diamCacheLEDs);
            if (isShowDevicePointSurfaces){
                mp_surfaceMain->drawDevicePointSurface();
            }
        }
    }
    else
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
        
        
/*        if (mp_gui1)
            mp_gui1->draw();
*/
    }
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
    
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
void testApp::guiUpdateListDevices()
{
    if (mp_deviceManager && mp_ddlDevices)
    {
        mp_ddlDevices->clearToggles();
        vector<string> listDevicesId;
        mp_deviceManager->getDevicesListId(listDevicesId);
//        printf("%d devices\n", (int)listDevicesId.size());
        
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
    if (mp_deviceSimulator)
        mp_deviceSimulator->audioIn(input, bufferSize, nChannels);
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
void testApp::mousePressed(int x, int y, int button)
{
    if (!isViewSimulation)
    {
        if (mp_guiAnimProps && mp_guiAnimProps->isHit(x,y))
            return;

        
        
        Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
        
        if (pDeviceCurrent)
        {
            Surface* pSurfaceCurrent = getSurfaceForDevice(pDeviceCurrent);
            if (pSurfaceCurrent)
            {
                pDeviceCurrent->setPointSurface(float(x)/pSurfaceCurrent->getWidthPixels(), float(y)/pSurfaceCurrent->getHeightPixels());
            
            
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
    else
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
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if (!isViewSimulation)
    {
        
    }
    else
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

