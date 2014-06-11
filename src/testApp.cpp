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

#include "tools.h"

//--------------------------------------------------------------
void testApp::setup()
{
	// Init
	m_isUpdateLayout = false;
	
    // Settings
    m_settings.loadFile("configuration.xml");
    int simulator = m_settings.getValue("murmur:simulator:enable",1);
 
    mp_deviceSimulator = 0;
    mp_deviceManager = new DeviceManager();
    isFullscreen = false;
    isShowDevicePointSurfaces = false;
    isAnimationSequence = false;
    isViewSimulation = true;
    isSimulation =  simulator == 1 ? true : false;
 
    // Globals
    Globals::instance()->mp_app = this;
    Globals::instance()->mp_deviceManager = mp_deviceManager;
	
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
	toolConfiguration* 	pToolConfiguration 	= new toolConfiguration(&toolManager);
	toolAnimations* 	pToolAnimations 	= new toolAnimations(&toolManager);
	toolDevices*		pToolDevices		= new toolDevices(&toolManager, mp_deviceManager);
	toolNetwork* 		pToolNetwork 		= new toolNetwork(&toolManager);

	toolManager.addTool( pToolConfiguration );
	toolManager.addTool( new toolNetwork(&toolManager) );
	toolManager.addTool( pToolDevices );
	toolManager.addTool( new toolSurfaces(&toolManager, mp_surfaceMain) );
	toolManager.addTool( pToolAnimations );
	toolManager.addTool( new toolScene(&toolManager, mp_sceneVisualisation) );
	toolManager.addTool( new toolSound(&toolManager) );

	toolManager.setLogo("murmur_logo.png");
	toolManager.setFontName("Fonts/NewMedia Fett.ttf");
	toolManager.createControls(ofVec2f(100,100),ofVec2f(200,200));
	toolManager.loadData();
 
    // Run network
	// if (pToolNetwork) 			pToolNetwork->setup();
	m_oscReceiver.setup(1234);
	
	// Simulators
	initSimulators();

	// setup
	if (pToolAnimations)		pToolAnimations->createControlsAnimations(mp_surfaceMain);
	if (pToolConfiguration)		pToolConfiguration->setup();
	if (pToolDevices)			pToolDevices->setup();
	
	// GO
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
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

	toolManager.saveData();
	
    delete mp_sceneVisualisation;
    delete mp_deviceManager;
    delete Globals::instance();
    delete Data::instance();

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
/*void testApp::updateControls()
{
	if (mp_lblSurfaceActivity)
	{
	    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
		if (pSurfaceCurrent)
			mp_lblSurfaceActivity->setLabel(pSurfaceCurrent->getStateActivity() + " - " + ofToString( pSurfaceCurrent->getVolumePacketsHistoryMean() ));
	}

}
*/

//--------------------------------------------------------------
void testApp::initJS()
{
	int err = ofxJSInitialize();
    if (err == 0)
	{
		// Log
		// ofxJSPrintCallback((void*)this, logJS);

		// Create some new functions
        setupJS();
	}
}
//--------------------------------------------------------------
void testApp::update()
{
	// dt
	float dt = (float) ofGetLastFrameTime();

	// Surface
	// TODO : put this in toolSurfaces
	if (mp_surfaceMain)
		mp_surfaceMain->update(dt);

	// Tools
 	toolManager.update();
 	m_oscReceiver.update();
 
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
	
	// updateControls();
}

//--------------------------------------------------------------
void testApp::draw()
{
    if (isViewSimulation)
    {
        // Draw Scene
        if (mp_sceneVisualisation)
            mp_sceneVisualisation->draw();

		if (m_isUpdateLayout)
		{
			guiUpdateListDevices();
			m_isUpdateLayout = false;
		}
		
		toolManager.draw();
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
void testApp::selectDeviceWithIndex(int index)
{
	toolDevices* pToolDevices = (toolDevices*) toolManager.getTool("Devices");
	if (pToolDevices)
		pToolDevices->selectDeviceWithIndex(index);
}

//--------------------------------------------------------------
void testApp::selectDevice(string id)
{
	toolDevices* pToolDevices = (toolDevices*) toolManager.getTool("Devices");
	if (pToolDevices)
		pToolDevices->selectDevice(id);
}

//--------------------------------------------------------------
void testApp::guiUpdateListDevices()
{
	toolDevices* pToolDevices = (toolDevices*) toolManager.getTool("Devices");
	if (pToolDevices)
	{
		pToolDevices->updateListDevices();
	}
}

//--------------------------------------------------------------
/*void testApp::guiUpdateDeviceAnimationTitle()
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
*/


//--------------------------------------------------------------
void testApp::guiUpdateViewSimulation()
{
/*    if (mp_tgViewSimu)
        mp_tgViewSimu->setValue( isViewSimulation );
*/
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
    
//    mp_guiAnimProps = 0;
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
/*	saveWidgetState(mp_guiMain);
	saveWidgetState(mp_gui1);
	saveWidgetState(mp_gui2);
	saveWidgetState(mp_gui3);
	saveWidgetState(mp_gui4);
	saveWidgetState(mp_guiSound);
	saveWidgetState(mp_guiNetwork);
	saveWidgetState(mp_guiAnimations);
	saveWidgetState(mp_guiAnimProps);
*/
}

//--------------------------------------------------------------
void testApp::restoreControlsState()
{
/*	if (mp_guiMain) mp_guiMain->setVisible(m_mapControlsState[mp_guiMain]);
    if (mp_gui1) mp_gui1->setVisible(m_mapControlsState[mp_gui1]);
    if (mp_gui2) mp_gui2->setVisible(m_mapControlsState[mp_gui2]);
    if (mp_gui3) mp_gui3->setVisible(m_mapControlsState[mp_gui3]);
    if (mp_gui4) mp_gui4->setVisible(m_mapControlsState[mp_gui4]);
    if (mp_guiSound) mp_guiSound->setVisible(m_mapControlsState[mp_guiSound]);
    if (mp_guiNetwork) mp_guiNetwork->setVisible(m_mapControlsState[mp_guiNetwork]);
    if (mp_guiAnimations) mp_guiAnimations->setVisible(m_mapControlsState[mp_guiAnimations]);
    if (mp_guiAnimProps) mp_guiAnimProps->setVisible(m_mapControlsState[mp_guiAnimProps]);
*/
}

//--------------------------------------------------------------
void testApp::showControls(bool is)
{
/*
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
        //toggleView();
        //guiUpdateViewSimulation();
	    isViewSimulation = !isViewSimulation;
		toolManager.enableDrawCallback( isViewSimulation );
    }
	else
	{
		toolManager.keyPressed(key);
	}


/*	if (key == 'e')
	{
        Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
        if (pSurfaceCurrent && pSurfaceCurrent->getAnimationManager().mp_animationCurrent)
		{
			string imgName = "Exports/Images/" + ofToString( ofGetUnixTime() ) + "_export_"+pSurfaceCurrent->getAnimationManager().mp_animationCurrent->m_name + ".png";

			ofSaveScreen(imgName);
		}
	
	}
	else
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
    {
        if (mp_sceneVisualisation)
            mp_sceneVisualisation->keyPressed(key);
    }
*/
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
	if (isViewSimulation)
	{
        m_isUserControls = toolManager.isHit(x,y);
        if (m_isUserControls == false)
		{
            mp_sceneVisualisation->mousePressed(x, y);
		}
	}
	else
	{
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

