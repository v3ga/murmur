//
//  toolSurfaces.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolSurfaces.h"
#include "toolDevices.h"
#include "toolScene.h"
#include "globals.h"
#include "testApp.h"

//--------------------------------------------------------------
toolSurfaces::toolSurfaces(toolManager* parent, Surface* surface) : tool("Surfaces", parent)
{
	mp_lblSurfaceActivity 	= 0;
	mp_surfaceMain			= surface;
}




	
//--------------------------------------------------------------
void toolSurfaces::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float widthDefault = 320;
		float dim = 16;
	
	    mp_canvas->addWidgetDown	( new ofxUILabel("Surfaces", OFX_UI_FONT_MEDIUM) );
    	mp_canvas->addWidgetDown	( new ofxUISpacer(widthDefault, 2));

	    mp_canvas->addWidgetDown	( new ofxUIToggle("surf. enable standby", false, dim, dim) );
    	mp_canvas->addWidgetRight	( new ofxUILabel("| state", fontType) );
		mp_lblSurfaceActivity = new ofxUILabel("1234", "4",fontType);
    	mp_canvas->addWidgetRight 	( mp_lblSurfaceActivity );
    	mp_canvas->addWidgetDown	(new ofxUISlider( "th. go standby", 0.01f, 0.1f, 0.02f,widthDefault-10, dim ));
    	mp_canvas->addWidgetDown	(new ofxUISlider( "th. go active", 0.01f, 0.1f, 0.02f,widthDefault-10, dim ));
    	mp_canvas->addWidgetDown	(new ofxUISlider( "dur. pre-standby", 10.0f, 60.0f, 20.0f,widthDefault-10, dim ));


    	mp_canvas->addWidgetDown	( new ofxUILabel("Dimensions", fontType) );
    	mp_canvas->addWidgetRight 	( new ofxUITextInput("wSurface", "4" , 40, dim,0,0,fontType));
    	mp_canvas->addWidgetRight	( new ofxUILabel("x", fontType) );
    	mp_canvas->addWidgetRight	( new ofxUITextInput("hSurface", "3" , 40, dim,0,0,fontType));

    	mp_canvas->addWidgetDown	( new ofxUILabel("FBO", fontType) );
    	mp_canvas->addWidgetRight 	( new ofxUITextInput("wFboSurface", "1000" , 40, dim,0,0,fontType));
    	mp_canvas->addWidgetRight	( new ofxUILabel("x", fontType) );
		mp_canvas->addWidgetRight	( new ofxUITextInput("hFboSurface", "1000" , 40, dim,0,0,fontType));


	    mp_canvas->addWidgetDown	(new ofxUIToggle("syphon", false, dim, dim));
    	mp_canvas->addWidgetRight	(new ofxUIToggle("target", false, dim, dim));
    	mp_canvas->addWidgetRight	(new ofxUISlider("target line w", 1.0f, 8.0f, 2.0f, 100, dim ));
		
		mp_canvas->autoSizeToFitWidgets();
	
	}
}

//--------------------------------------------------------------
Surface* toolSurfaces::getSurfaceForDevice(Device* pDevice)
{
    // TODO : iterate through a liste of surfaces to find the surface that has the device attached to it
    return mp_surfaceMain;
}

//--------------------------------------------------------------
Surface* toolSurfaces::getSurfaceForDeviceCurrent()
{
	toolDevices*	pToolDevices = (toolDevices*) mp_toolManager->getTool("Devices");
	if (pToolDevices && pToolDevices->mp_deviceManager)
	    return getSurfaceForDevice( pToolDevices->mp_deviceManager->getDeviceCurrent() );
	return 0;
}

//--------------------------------------------------------------
void toolSurfaces::onSurfaceModified(Surface* pSurface)
{
	toolScene* pToolScene = (toolScene*)mp_toolManager->getTool("Scene");

	if(pToolScene && pToolScene->getScene() && pSurface)
    {
   		vector<Device*>& listDevices = pSurface->getListDevices();
   		vector<Device*>::iterator it = listDevices.begin();
   		for ( ; it != listDevices.end() ; ++it)
   		{
			DeviceNode* pDeviceNode = pToolScene->getScene()->getDeviceNode(*it);
            SurfaceNode* pSurfaceNode = pToolScene->getScene()->getSurfaceNode(pSurface);
			if (pDeviceNode && pSurfaceNode)
            	pDeviceNode->setPositionNodeSurface( pSurfaceNode->getGlobalPositionDevicePointSurface(pDeviceNode->getDevice()) );
   		}
	}
}

//--------------------------------------------------------------
void toolSurfaces::update()
{
    if (mp_surfaceMain)
	{
        mp_surfaceMain->renderOffscreen(GLOBALS->mp_app->isShowDevicePointSurfaces);
		mp_surfaceMain->publishSyphon();
    }
}

//--------------------------------------------------------------
void toolSurfaces::handleEvents(ofxUIEventArgs& e)
{
    Surface* pSurfaceCurrent = getSurfaceForDeviceCurrent();
	if (pSurfaceCurrent == 0) return;

	toolScene* pToolScene = (toolScene*)mp_toolManager->getTool("Scene");
	if (pToolScene == 0) return;


    string name = e.widget->getName();
	if (name == "surf. enable standby")
	{
		pSurfaceCurrent->setEnableStandby( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else if (name == "th. go standby")
	{
		pSurfaceCurrent->setGoStandbyTh( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else if (name == "th. go active")
	{
		pSurfaceCurrent->setGoActiveTh( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else if (name == "dur. pre-standby")
	{
		pSurfaceCurrent->setDurationPreStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
	else if (name == "wSurface")
    {
		SurfaceNode * pSurfaceNodeCurrent = pToolScene->getSurfaceNode(pSurfaceCurrent);
		float wSurface = atof( ((ofxUITextInput *) e.widget)->getTextString().c_str() );
		pSurfaceNodeCurrent->setWidth(wSurface);
		pSurfaceNodeCurrent->setPosition(-0.5f*wSurface, pSurfaceNodeCurrent->getPosition().y, pSurfaceNodeCurrent->getPosition().z);
  
		onSurfaceModified(pSurfaceCurrent);
	}
    else if (name == "hSurface")
    {
		SurfaceNode * pSurfaceNodeCurrent = pToolScene->getSurfaceNode(pSurfaceCurrent);
		pSurfaceNodeCurrent->setHeight( atof( ((ofxUITextInput *) e.widget)->getTextString().c_str() ) );
		
		onSurfaceModified(pSurfaceCurrent);
	}
    else if (name == "wFboSurface")
    {
		int wPixels = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );;
		int hPixels = pSurfaceCurrent->getHeightPixels();
		pSurfaceCurrent->setDimensions(wPixels,hPixels);
	}
    else if (name == "hFboSurface")
    {
		int wPixels = pSurfaceCurrent->getWidthPixels();
		int hPixels = atoi( ((ofxUITextInput *) e.widget)->getTextString().c_str() );;
		pSurfaceCurrent->setDimensions(wPixels,hPixels);
	}
    else if (name == "syphon")
	{
		pSurfaceCurrent->setPublishSyphon( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else if (name == "target")
	{
		pSurfaceCurrent->setRenderTarget( ((ofxUIToggle *) e.widget)->getValue() );
	}
	else if (name == "target line w")
	{
		pSurfaceCurrent->setRenderTargetLineWidth( ((ofxUISlider *) e.widget)->getScaledValue() );
	}
}


