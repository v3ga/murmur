//
//  animationComposition.cpp
//  murmur
//
//  Created by Julien on 25/06/2015.
//
//

#include "animationComposition.h"
#include "animationsFactory.h"
#include "globals.h"

//--------------------------------------------------------------
AnimationComposition::AnimationComposition(string name) : Animation(name)
{

}


//--------------------------------------------------------------
void AnimationComposition::createUICustom()
{
	if (m_animations.size()>0)
	{
	
/*
		m_animations[0]->mp_UIcanvas = new ofxUICanvas(mp_UIcanvas->getRect()->getWidth(), mp_UIcanvas->getRect()->getHeight());

		m_animations[0]->createUICustom();

		mp_UIcanvas->addWidgetDown(m_animations[0]->mp_UIcanvas);
		m_animations[0]->mp_UIcanvas->setVisible(true);
*/

	}
}

//--------------------------------------------------------------
void AnimationComposition::add(string name)
{
	// limit to 2 animations for the moment
	if (m_animations.size()<2)
	{
		Animation* pAnimation = AnimationsFactory::create(name);
		if (pAnimation){
			//pAnimation->setDrawBackground(false);
			m_animations.push_back(pAnimation);
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::loadProperties(string id)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->loadProperties( id+"_"+(*it)->m_name );
}

//--------------------------------------------------------------
void AnimationComposition::VM_enter()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_enter();
 
	Surface* pSurface = GLOBALS->getSurfaceMain();
 	if (pSurface)
		pSurface->setRenderOffscreenCb((Surface::FRenderOffscreen)onSurfaceRenderOffscreen, this);

	
	if (!m_fboAnimation1.isAllocated()) m_fboAnimation1.allocate(2560,1024, GL_RGBA);
	if (!m_fboAnimation2.isAllocated()) m_fboAnimation2.allocate(2560,1024, GL_RGBA);

	m_shader.load("Shaders/animCompo.vert", "Shaders/animCompoMultiply.frag");
}

//--------------------------------------------------------------
void AnimationComposition::VM_update(float dt)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_update(dt);
}

//--------------------------------------------------------------
void AnimationComposition::VM_draw(float w, float h)
{
/*
	drawBackground(0);
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	{
		ofPushStyle();
		ofPushMatrix();
	 	(*it)->VM_draw(w,h);
		ofPopMatrix();
		ofPopStyle();
	}
*/
}

//--------------------------------------------------------------
void AnimationComposition::VM_exit()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_exit();

	Surface* pSurface = GLOBALS->getSurfaceMain();
 	if (pSurface)
		pSurface->setRenderOffscreenCb(0,0);
}


//--------------------------------------------------------------
void AnimationComposition::onNewPacket(DevicePacket* pDevice, string deviceId, float x, float y)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
		(*it)->onNewPacket(pDevice,deviceId,x,y);
}

//--------------------------------------------------------------
void AnimationComposition::onSurfaceRenderOffscreen(Surface* pSurface, AnimationComposition* pThis)
{
	ofFbo& fboFinal = pSurface->getOffscreen();
	
	
	
	if (pThis->m_animations.size()==2)
	{
		pThis->m_fboAnimation1.begin();
		pThis->m_animations[0]->VM_draw(pThis->m_fboAnimation1.getWidth(), pThis->m_fboAnimation1.getHeight());
		pThis->m_fboAnimation1.end();

		pThis->m_fboAnimation2.begin();
		pThis->m_animations[1]->VM_draw(pThis->m_fboAnimation2.getWidth(), pThis->m_fboAnimation2.getHeight());
		pThis->m_fboAnimation2.end();
	}

	
	fboFinal.begin();

	pThis->m_shader.begin();
	pThis->m_shader.setUniformTexture("tex1", pThis->m_fboAnimation1.getTextureReference(), 1);
	pThis->m_shader.setUniformTexture("tex2", pThis->m_fboAnimation2.getTextureReference(), 2);
	pThis->m_fboAnimation1.draw(0,0);
	//	pThis->m_animations[0]->VM_draw(fboFinal.getWidth(), fboFinal.getHeight());
	pThis->m_shader.end();

	fboFinal.end();

}



