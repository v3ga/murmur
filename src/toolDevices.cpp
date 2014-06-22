//
//  toolDevices.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolDevices.h"
#include "globals.h"
#include "testApp.h"
#include "toolSurfaces.h"
#include "toolAnimations.h"

//--------------------------------------------------------------
toolDevices::toolDevices(toolManager* parent, DeviceManager* manager) : tool("Devices", parent)
{
	mp_ddlDevices 	= 0;
	mp_deviceManager= manager;
	mp_canvasDevice = 0;
	mp_lblDeviceTitle = 0;
	mp_sliderDeviceVolMax = 0;
	mp_sliderDeviceVolHistorySize = 0;
	mp_toggleDeviceEnableStandby = 0;
	mp_sliderDeviceVolHistoryTh = 0;
	mp_sliderDeviceTimeStandby = 0;
//	mp_sliderDeviceNbLEDsStandby = 0;
//	mp_sliderDeviceSpeedStandby = 0;
	mp_sliderDeviceSampleVolStandby = 0;
}

//--------------------------------------------------------------
void toolDevices::setup()
{
	if (GLOBALS->mp_app->isSimulation){
		selectDeviceWithIndex(0);
	}
}

//--------------------------------------------------------------
void toolDevices::selectDeviceWithIndex(int index)
{
    if (mp_deviceManager && index>=0 && index < mp_deviceManager->m_listDevices.size())
		selectDevice(mp_deviceManager->m_listDevices[index]->m_id);
}


//--------------------------------------------------------------
void toolDevices::show(bool is)
{
	tool::show(is);
	if (mp_canvasDevice)
		mp_canvasDevice->setVisible(is);
}

//--------------------------------------------------------------
void  toolDevices::enableDrawCallback(bool is)
{
	tool::enableDrawCallback(is);
	if (mp_canvasDevice){
		if (is){
			mp_canvasDevice->enableAppDrawCallback();
			mp_canvasDevice->enableMouseEventCallbacks();
		}else{
			mp_canvasDevice->disableAppDrawCallback();
			mp_canvasDevice->disableMouseEventCallbacks();
		}
	}
}

//--------------------------------------------------------------
void toolDevices::createControlsCustom()
{
	if (mp_canvas)
	{
		float widthDefault = 320;
	
	    vector<string> listDevicesIds;
		mp_ddlDevices = new ofxUIDropDownList("Devices", listDevicesIds);
    	updateListDevices();
		mp_canvas->addWidgetDown(mp_ddlDevices);
	}
}

//--------------------------------------------------------------
void toolDevices::createControlsCustomFinalize()
{
	 float widthDefault = 320;
	 float dim = 16;
	 ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;

	 ofVec2f pos(
		 getCanvas()->getRect()->getX() + getCanvas()->getRect()->getWidth()+10,
		 getCanvas()->getRect()->getY()
	 );

	mp_canvasDevice = new ofxUICanvas(pos.x,pos.y,widthDefault,300);
	
	mp_canvasDevice->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	mp_lblDeviceTitle = new ofxUILabel("Devices", OFX_UI_FONT_LARGE);
    mp_canvasDevice->addWidgetDown(mp_lblDeviceTitle);
    mp_canvasDevice->addWidgetDown(new ofxUISpacer(widthDefault, 2));
	
	if (GLOBALS->mp_app->isSimulation)
	{
		mp_canvasDevice->addWidgetDown( new ofxUIToggle("mute sound input", false, dim, dim) );
	}
 
    mp_sliderDeviceVolMax = new ofxUISlider("Vol. max", 0.005f, 0.04f, 0.02f, widthDefault-10, dim );
    mp_sliderDeviceVolHistorySize = new ofxUISlider("Vol. history size", 50, 500, 400, widthDefault-10, dim );
    mp_toggleDeviceEnableStandby = new ofxUIToggle("Enable standby", true, dim, dim);
    mp_sliderDeviceVolHistoryTh = new ofxUISlider( "Vol. history standby", 0.0f, 0.75f, 0.5f, widthDefault-10, dim );
    mp_sliderDeviceTimeStandby = new ofxUISlider("Time standby", 5.0f, 60.0f, 10.0f, widthDefault-10, dim );
	mp_sliderDeviceSampleVolStandby = new ofxUISlider( "Sample vol. standby", 0.0f, 1.0f, 0.35f, widthDefault-10, dim );
//    mp_sliderDeviceNbLEDsStandby = new ofxUISlider( "Nb LEDs standby", 10, 100, 50.0f, widthDefault-10, dim );
//    mp_sliderDeviceSpeedStandby = new ofxUISlider( "Speed standby", 40, 360, 70.0f, widthDefault-10, dim );
 
	mp_canvasDevice->addWidgetDown(mp_sliderDeviceVolMax);
	mp_canvasDevice->addWidgetDown(mp_sliderDeviceVolHistorySize);

    mp_canvasDevice->addWidgetDown(new ofxUILabel("> Stand by", fontType));
    mp_canvasDevice->addWidgetDown(mp_toggleDeviceEnableStandby);
	mp_canvasDevice->addWidgetDown(mp_sliderDeviceVolHistoryTh);
	mp_canvasDevice->addWidgetDown(mp_sliderDeviceTimeStandby);
	mp_canvasDevice->addWidgetDown(mp_sliderDeviceSampleVolStandby);
//	mp_canvasDevice->addWidgetDown(mp_sliderDeviceNbLEDsStandby);
//	mp_canvasDevice->addWidgetDown(mp_sliderDeviceSpeedStandby);

	mp_canvasDevice->autoSizeToFitWidgets();
	
	ofAddListener(mp_canvasDevice->newGUIEvent, this, &toolDevices::handleEvents);
}

//--------------------------------------------------------------
void toolDevices::drawUI()
{
	Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
	if (pDeviceCurrent)
	{
		pDeviceCurrent->drawSoundInputVolume( 0.5f*(ofGetWidth()) ,ofGetHeight()-pDeviceCurrent->getHeightSoundInputVolume());
    }
}

//--------------------------------------------------------------
void toolDevices::updateListDevices()
{
    if (mp_deviceManager && mp_ddlDevices)
    {
        vector<string> listDevicesId;
        mp_deviceManager->getDevicesListId(listDevicesId);

        printf("[toolDevices::guiUpdateListDevices]\n  - %d device(s)\n", (int)listDevicesId.size());
     
		mp_ddlDevices->setAutoClose(false);
        mp_ddlDevices->init("Devices",listDevicesId);
    }
}

//--------------------------------------------------------------
void toolDevices::selectDevice(string id)
{
    if (mp_deviceManager)
    {
        Device* pDevice = mp_deviceManager->setDeviceCurrent( id );
        if (pDevice)
        {
            // Label of container
            if (mp_lblDeviceTitle) mp_lblDeviceTitle->setLabel(pDevice->m_id);
        
            // Data
            updateDeviceUI(pDevice);
        }
    }
}

//--------------------------------------------------------------
void toolDevices::updateDeviceAnimationTitle()
{
	toolSurfaces* pToolSurface = (toolSurfaces*) mp_toolManager->getTool("Surfaces");
	if (pToolSurface == 0) return;

	toolAnimations* pToolAnimation = (toolAnimations*) mp_toolManager->getTool("Animations");
	if (pToolAnimation == 0) return;

   Surface* pSurfaceCurrent = pToolSurface->getSurfaceForDeviceCurrent();
   if (pSurfaceCurrent)
   {
   	 if (pToolAnimation->mp_lblAnimDirJs)
	 {
			 pToolAnimation->mp_lblAnimDirJs->setLabel("> dir "+pSurfaceCurrent->m_strDirScripts);
	 }
   
	 if (pToolAnimation->mp_lblAnimTitle)
	 {
	 	if (pSurfaceCurrent->getAnimationManager().mp_animationCurrent)
	 		pToolAnimation->mp_lblAnimTitle->setLabel("> surface '"+pSurfaceCurrent->getId()+"' / playing '"+pSurfaceCurrent->getAnimationManager().mp_animationCurrent->m_name+"'");
	 }
	}

}

//--------------------------------------------------------------
void toolDevices::updateDeviceUI(Device* pDevice)
{
    if (pDevice)
    {
        if (mp_sliderDeviceVolMax) mp_sliderDeviceVolMax->setValue( pDevice->getSoundInputVolumeMax() );
        if (mp_sliderDeviceVolHistorySize) mp_sliderDeviceVolHistorySize->setValue( float(pDevice->getSoundInputVolHistorySize()) );
        if (mp_sliderDeviceVolHistoryTh) mp_sliderDeviceVolHistoryTh->setValue( float(pDevice->getSoundInputVolHistoryTh()) );
        if (mp_toggleDeviceEnableStandby) mp_toggleDeviceEnableStandby->setValue( pDevice->getEnableStandbyMode() );
        if (mp_sliderDeviceTimeStandby) mp_sliderDeviceTimeStandby->setValue( pDevice->getTimeStandby() );
        if (mp_sliderDeviceSampleVolStandby) mp_sliderDeviceSampleVolStandby->setValue( pDevice->getSampleVolStandby() );

		 updateDeviceAnimationTitle();
    }
}


//--------------------------------------------------------------
void toolDevices::handleEvents(ofxUIEventArgs& e)
{
    Device* pDeviceCurrent = mp_deviceManager->getDeviceCurrent();
	if (pDeviceCurrent == 0) return;

    string name = e.widget->getName();

    if (name == "Devices")
    {
		ofLog()  <<  "ddl Devices";
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        if (selected.size()==1)
            selectDevice( selected[0]->getName() );
    }
	else if (name == "mute sound input")
	{
		if (pDeviceCurrent)
			pDeviceCurrent->mute( ((ofxUIToggle *) e.widget)->getValue() );
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
    else if (name == "Time standby")
    {
        if (pDeviceCurrent){
            pDeviceCurrent->setTimeStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
        }
    }
    else if (name == "Sample vol. standby")
    {
        if (pDeviceCurrent){
            pDeviceCurrent->setSampleVolumeStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
        }
    }
    else if (name == "Speed standby")
    {
        if (pDeviceCurrent){
 //           pDeviceCurrent->setSpeedStandby( ((ofxUISlider *) e.widget)->getScaledValue() );
        }
    }
}

//--------------------------------------------------------------
bool toolDevices::isHit(int x, int y)
{
	return ( tool::isHit(x,y) || mp_canvasDevice->isHit(x, y));
}

