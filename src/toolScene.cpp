//
//  toolScene.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolScene.h"
#include "globals.h"

//--------------------------------------------------------------
toolScene::toolScene(toolManager* parent, SceneVisualisation* scene) : tool("Scene", parent)
{
	mp_sceneVisualisation = scene;
	m_isDrawDeviceLightCurrent = true;
}
	
//--------------------------------------------------------------
void toolScene::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float dim = 16;
		int widthDefault = 320;

	    mp_canvas->addWidgetDown( new ofxUILabel("Scene",OFX_UI_FONT_LARGE) );
    	mp_canvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );

		mp_canvas->addWidgetDown( new ofxUIToggle("draw lights for current device only", &m_isDrawDeviceLightCurrent,dim, dim) );

		mp_canvas->autoSizeToFitWidgets();
	}
}

//--------------------------------------------------------------
SurfaceNode* toolScene::getSurfaceNode(Surface* pSurface)
{
	if (mp_sceneVisualisation)
		return mp_sceneVisualisation->getSurfaceNode(pSurface);
	return 0;
}


//--------------------------------------------------------------
void toolScene::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
	if (name =="draw lights for current device only"){
		setDeviceCurrentNodeDrawLights();
	}
}

//--------------------------------------------------------------
void toolScene::setDeviceCurrentNodeDrawLights()
{
  vector<Device*> listDevices = GLOBALS->mp_deviceManager->m_listDevices;

  for (int i=0; i<listDevices.size(); i++ )
  {
	  Device* pDevice = listDevices[i];
	  DeviceNode* pDeviceNode = mp_sceneVisualisation->getDeviceNodeFor(pDevice);
	  if (m_isDrawDeviceLightCurrent)
	  {
		  if (pDevice == GLOBALS->mp_deviceManager->getDeviceCurrent())	pDeviceNode->setDrawLights(true);
		  else pDeviceNode->setDrawLights(false);
	  }
	  else
	  {
		  pDeviceNode->setDrawLights(true);
	  }
  }
}

//--------------------------------------------------------------
void toolScene::onDeviceChanged()
{
	setDeviceCurrentNodeDrawLights();
}



