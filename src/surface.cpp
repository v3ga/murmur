/*
 *  surface.cpp
 *  murmur
 *
 *  Created by Julien on 10/04/13.
 *  Copyright 2013 2Roqs. All rights reserved.
 *
 */

#include "surface.h"
#include "globals.h"
#include "ofxXmlSettings.h"
#include "animationsFactory.h"

//--------------------------------------------------------------
Surface::Surface(string id, int wPixels, int hPixels)
{
    m_id = id;
	setDimensions(wPixels,hPixels);
    printf(">>> new Surface(%d,%d)\n", wPixels, hPixels);

    m_durationTransition = 1.0f;
    m_durationAnimation  = 5.0f;
    m_isTimelineActive   = false;
    m_isAnimationTransiting = false;
    m_timeAnimation = 0.0f;
}

//--------------------------------------------------------------
void Surface::setTimelineActive(bool is)
{
    m_isTimelineActive = is;
    m_timeAnimation = 0.0f;
    m_animationManager.m_animationInfoIndex = 0;
    m_animationManager.setAnimationTimelineNext();
}

//--------------------------------------------------------------
void Surface::setup()
{
    // Cpp animations
    ofxXmlSettings surfaceSettings;
    if ( surfaceSettings.loadFile("Config/surfaceMain_animations.xml") ) // TODO : depends on the id of the surface...
    {
        surfaceSettings.pushTag("surface");
        surfaceSettings.pushTag("animations");

        int nbAnimations = surfaceSettings.getNumTags("animation");
        printf("Surface >>> nbAnimations=%d\n", nbAnimations);
        for (int i=0;i<nbAnimations;i++){
            
            string animName = surfaceSettings.getValue("animation","",i);
            printf("   - %s\n", animName.c_str());
            
            if (animName != ""){
                Animation* pAnimation = AnimationsFactory::create( animName );
                if (pAnimation!=0)
                    m_animationManager.M_addAnimation(pAnimation);
            }
        }
        surfaceSettings.popTag();
        surfaceSettings.popTag();
    }
    else
        printf("Surface >>> cannot load %s\n", "surfaceMain_animations.xml");
    
	// Scripts
	ofDirectory dirScripts("Scripts");
	if (dirScripts.exists())
	{
		dirScripts.listDir();
		printf("DIR %s [%d file(s)]\n", dirScripts.path().c_str(),dirScripts.size());
        
		vector<ofFile> files = dirScripts.getFiles();
		vector<ofFile>::iterator it;
		for (it = files.begin(); it != files.end(); ++it)
        {
            if ((*it).getExtension() == "js")
			{
				Animation* pAnimation = new Animation((*it).getFileName(), (*it).getAbsolutePath());
				
				// Eval script to extract informations from it
				if (pAnimation->M_loadScript( (*it).getAbsolutePath().c_str() ))
                {
					// Theme + autoclear
					ofxJSValue retValTheme,retValAutoClear;
					retValTheme = int_TO_ofxJSValue(1);
					retValAutoClear = bool_TO_ofxJSValue(true);
                    
                    if (pAnimation->mp_obj)
                    {
						ofxJSCallFunctionNameObject_NoArgs_IfExists(pAnimation->mp_obj,"getTheme",		retValTheme);
						ofxJSCallFunctionNameObject_NoArgs_IfExists(pAnimation->mp_obj,"getAutoClear",	retValAutoClear);
					}
                    
					int theme = ofxJSValue_TO_int(retValTheme);
					bool isAutoClear = ofxJSValue_TO_bool(retValAutoClear);
					
                    printf("- [%s], theme=%d, autoclear=%s\n", (*it).getFileName().c_str(),theme, isAutoClear ? "true" : "false");
                    
					pAnimation->M_setTheme(theme);
					pAnimation->m_isAutoClear = isAutoClear;

					m_animationManager.M_addAnimation(pAnimation);
				}
			}
		}
	}
	dirScripts.close();
    
    m_animationManager.M_readSettings(surfaceSettings);
    m_animationManager.M_setAnimation(0);
}

//--------------------------------------------------------------
void Surface::setDimensions(int w, int h)
{
	m_fbo.allocate(w,h);
}

//--------------------------------------------------------------
void Surface::addDevice(Device* pDevice)
{
    m_listDevices.push_back(pDevice);
}

//--------------------------------------------------------------
void Surface::update(float dt)
{
    m_animationManager.M_update(dt);
    if (m_isTimelineActive)
    {
        m_timeAnimation += dt;
        //printf("tAnim=%.2f / tAnimDuration=%.2f\n", m_timeAnimation, m_durationAnimation);
        
        if (m_timeAnimation>=m_durationAnimation && !m_animationManager.M_isTransiting())
        {
           m_animationManager.setAnimationTimelineNext();
            m_isAnimationTransiting = true;
            m_timeAnimation = 0.0f;
        }

        if (/*m_isAnimationTransiting && */m_animationManager.M_isTransiting() == false){
//            printf(">>> end of transition\n");
//            m_isAnimationTransiting = false;
        }
    
    }
}
//--------------------------------------------------------------
void Surface::renderOffscreen()
{
    m_fbo.begin();
    m_animationManager.M_drawCanvas(m_fbo.getWidth(),m_fbo.getHeight());
    m_fbo.end();
}

//--------------------------------------------------------------
void Surface::drawDevicePointSurface()
{
    ofPushStyle();
    ofSetColor(255,255,255,255);
    
    vector<Device*>::iterator itDevices = m_listDevices.begin();
    ofVec2f posSurface;
    Device* pDevice=0;
    for (;itDevices!=m_listDevices.end();++itDevices)
    {
        pDevice = *itDevices;
        posSurface.set(pDevice->m_pointSurface.x*m_fbo.getWidth(), pDevice->m_pointSurface.y*m_fbo.getHeight());

        ofLine(posSurface.x,0.0f, posSurface.x,m_fbo.getHeight());
        ofLine(0.0f,posSurface.y, m_fbo.getWidth(),posSurface.y);
    }

    ofPopStyle();
}

//--------------------------------------------------------------
void Surface::drawCacheLEDs(float d)
{
    // TEMP
    return;
    
    ofPushStyle();
    ofSetColor(255,255,255,255);

    vector<Device*>::iterator itDevices = m_listDevices.begin();
    ofVec2f posSurface;
    Device* pDevice=0;
    for (;itDevices!=m_listDevices.end();++itDevices)
    {
        pDevice = *itDevices;
        posSurface.set(pDevice->m_pointSurface.x*m_fbo.getWidth(), pDevice->m_pointSurface.y*m_fbo.getHeight());
        
        ofEllipse(posSurface.x,posSurface.y, d,d);
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void Surface::onNewPacket(DevicePacket* pDevicePacket, string deviceId)
{
    if (m_animationManager.mp_animationCurrent)
    {
        Device* pDevice = Globals::instance()->mp_deviceManager->getDeviceById(deviceId);
        if (pDevice)
            m_animationManager.mp_animationCurrent->onNewPacket(pDevicePacket, pDevice->m_id, pDevice->m_pointSurface.x*m_fbo.getWidth(), pDevice->m_pointSurface.y*m_fbo.getHeight());
    }
}

//--------------------------------------------------------------
void Surface::saveAnimationsProperties()
{
    vector<Animation*>::iterator it = m_animationManager.m_listAnimations.begin();
    Animation* pAnim;
    for ( ; it != m_animationManager.m_listAnimations.end(); ++it)
    {
        (*it)->saveProperties("surface"+this->m_id);
    }
}

//--------------------------------------------------------------
void Surface::loadAnimationsProperties()
{
    vector<Animation*>::iterator it = m_animationManager.m_listAnimations.begin();
    Animation* pAnim;
    for ( ; it != m_animationManager.m_listAnimations.end(); ++it)
    {
        (*it)->loadProperties("surface"+this->m_id);
    }
}





