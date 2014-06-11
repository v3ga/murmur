//
//  toolAnimations.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolAnimations.h"
#include "globals.h"
#include "testApp.h"
#include "toolSurfaces.h"

//--------------------------------------------------------------
toolAnimations::toolAnimations(toolManager* parent) : tool("Animations", parent)
{
	mp_lblAnimDirJs		= 0;
	mp_lblAnimTitle		= 0;
	mp_consoleJs		= 0;
	mp_animationUI		= 0;
	
	m_listLogJsMax		= 20;
}

//--------------------------------------------------------------
void toolAnimations::show(bool is)
{
	tool::show(is);
	if (is == false)
		showAnimationPropsAll(false);
	if (mp_animationUI && is)
		mp_animationUI->setVisible(true);
}

//--------------------------------------------------------------
void  toolAnimations::enableDrawCallback(bool is)
{
	// Call parent
	tool::enableDrawCallback(is);
	if (mp_animationUI){
		if (is){
			mp_animationUI->enableAppDrawCallback();
			mp_animationUI->enableMouseEventCallbacks();
		}else{
			mp_animationUI->disableAppDrawCallback();
			mp_animationUI->disableMouseEventCallbacks();
		}
	}
}



//--------------------------------------------------------------
void toolAnimations::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float widthDefault = 320;
		float dim = 16;

	
		mp_canvas->addWidgetDown(new ofxUILabel("Animations", OFX_UI_FONT_LARGE));
		mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 2));
		mp_canvas->addWidgetDown(new ofxUIToggle("activate sequence", GLOBALS->mp_app->isAnimationSequence, dim, dim));
		mp_canvas->addWidgetDown(new ofxUISlider("transition duration (s)", 0.5f, 2.0f, 0.75f, widthDefault-10, dim ));
		mp_canvas->addWidgetDown(new ofxUISlider("animation duration (s)", 30.0f, 180.0f, 120.0f, widthDefault-10, dim ));

		mp_canvas->addWidgetDown(new ofxUILabel("Scripts", OFX_UI_FONT_MEDIUM));
		mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 2));
		mp_lblAnimDirJs = new ofxUILabel("> dir", fontType);
		mp_lblAnimTitle = new ofxUILabel("> playing animation.js", fontType);
		mp_canvas->addWidgetDown(mp_lblAnimDirJs);
		mp_canvas->addWidgetDown(mp_lblAnimTitle);
		mp_canvas->addWidgetDown(new ofxUILabelButton("Reload", 100, false,fontType));
		mp_canvas->addWidgetRight(new ofxUILabelButton("Edit", 100, false,fontType));
		mp_canvas->addWidgetDown(new ofxUILabel("Console", OFX_UI_FONT_MEDIUM));
		mp_canvas->addWidgetDown(new ofxUISpacer(widthDefault, 2));
		mp_consoleJs = new ofxUITextArea("Console", "", widthDefault, 300,0,0,fontType);
		mp_canvas->addWidgetDown(mp_consoleJs);

		mp_canvas->autoSizeToFitWidgets();

	}
}

//--------------------------------------------------------------
void toolAnimations::createControlsAnimations(Surface* pSurface)
{
    if (pSurface)
	{
		float widthDefault = 320;
	
        AnimationManager& animManager = pSurface->getAnimationManager();
        vector<Animation*>& listAnimations = animManager.m_listAnimations;
        vector<Animation*>::iterator it = listAnimations.begin();
        for ( ; it != listAnimations.end() ; ++it)
        {
            Animation* pAnim = *it;

            ofxUIRectangle* pGuiAnimRect = mp_canvas->getRect();
            ofxUICanvas* pAnimCanvas = new ofxUICanvas(pGuiAnimRect->getX()+pGuiAnimRect->getWidth()+10, pGuiAnimRect->getY(), widthDefault, 300);
            pAnim->setUICanvas( pAnimCanvas );
            pAnim->createUI();
            
            pAnimCanvas->autoSizeToFitWidgets();
            pAnimCanvas->setVisible(false);
            
            m_mapAnimationUI[pAnim] = pAnimCanvas;
        }
        
        pSurface->loadAnimationsProperties();
    }
}

//--------------------------------------------------------------
void toolAnimations::logJS(void* pData, const string& message)
{
   std::cout << message;
   toolAnimations* pThis = (toolAnimations*) pData;
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
void toolAnimations::handleEvents(ofxUIEventArgs& e)
{
	toolSurfaces* pToolSurfaces = (toolSurfaces*) mp_toolManager->getTool("Surfaces");
	if (pToolSurfaces == 0) return;

    Surface* pSurfaceCurrent = pToolSurfaces->getSurfaceForDeviceCurrent();
	if (pSurfaceCurrent == 0) return 0;

    string name = e.widget->getName();

    if (name == "activate sequence")
    {
		pSurfaceCurrent->setTimelineActive( ((ofxUIToggle *) e.widget)->getValue() );
    }
    else if (name == "animation duration (s)")
    {
		pSurfaceCurrent->setDurationAnimation(  ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "transition duration (s)")
    {
		pSurfaceCurrent->setDurationTransition(  ((ofxUISlider *) e.widget)->getScaledValue() );
    }
    else if (name == "Reload")
    {
		pSurfaceCurrent->getAnimationManager().M_reloadScript();
    }
    else if (name == "Edit")
    {
		pSurfaceCurrent->getAnimationManager().M_editScript();
    }

}

//--------------------------------------------------------------
void toolAnimations::keyPressed(int key)
{
	toolSurfaces* pToolSurfaces = (toolSurfaces*) mp_toolManager->getTool("Surfaces");
	if (pToolSurfaces == 0) return;

	if (key == OF_KEY_RIGHT)
    {
        Surface* pSurfaceCurrent = pToolSurfaces->getSurfaceForDeviceCurrent();
        if (pSurfaceCurrent)
        {
            // Goto next animation
            pSurfaceCurrent->getAnimationManager().gotoAnimationNext();

            // Update Title
            updateDeviceAnimationTitle();

            // Show animation interface
            showAnimationPropsAll(false);
            if (!pSurfaceCurrent->isTimelineActive())
            {
                Animation* pAnimationCurrent = pSurfaceCurrent->getAnimationManager().mp_animationCurrent;
                if (pAnimationCurrent){
                    mp_animationUI = pAnimationCurrent->getUI();
					mp_animationUI->setVisible(true);
				 }
            }
        }
    }
	else
	if (key == OF_KEY_LEFT)
    {
        Surface* pSurfaceCurrent = pToolSurfaces->getSurfaceForDeviceCurrent();
        if (pSurfaceCurrent)
        {
            pSurfaceCurrent->getAnimationManager().gotoAnimationPrev();
            updateDeviceAnimationTitle();
        
            // Show animation interface
            showAnimationPropsAll(false);
            if (!pSurfaceCurrent->isTimelineActive())
            {
                Animation* pAnimationCurrent = pSurfaceCurrent->getAnimationManager().mp_animationCurrent;
                if (pAnimationCurrent)
				{
	                if (pAnimationCurrent){
    	                mp_animationUI = pAnimationCurrent->getUI();
						mp_animationUI->setVisible(true);
					 }
				}
            }
        }
    }
	
}

//--------------------------------------------------------------
void toolAnimations::showAnimationPropsAll(bool is)
{
	toolSurfaces* pToolSurfaces = (toolSurfaces*) mp_toolManager->getTool("Surfaces");
	if (pToolSurfaces == 0) return;

    Surface* pSurfaceCurrent = pToolSurfaces->getSurfaceForDeviceCurrent();
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
void toolAnimations::updateDeviceAnimationTitle()
{
	toolSurfaces* pToolSurfaces = (toolSurfaces*) mp_toolManager->getTool("Surfaces");
	if (pToolSurfaces == 0) return;

   Surface* pSurfaceCurrent = pToolSurfaces->getSurfaceForDeviceCurrent();
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

