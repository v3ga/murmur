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
	m_bAllocateRenderBuffers = true;
	m_bRenderNormal = true;
}


//--------------------------------------------------------------
void AnimationComposition::createUICustom()
{
}

//--------------------------------------------------------------
void AnimationComposition::loadMidiSettings()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->loadMidiSettings();
}

//--------------------------------------------------------------
void AnimationComposition::setRenderNormal(bool is)
{
	m_bRenderNormal = is;
	Surface* pSurface = GLOBALS->getSurfaceMain();


		for (int i=0;i<m_animations.size(); i++){
			m_animations[i]->setDrawBackground(!is);
		}


	if (m_bRenderNormal)
	{
	 	if (pSurface)
			pSurface->setRenderOffscreenCb(0,0);

	}
	else
	{
	 	if (pSurface)
			pSurface->setRenderOffscreenCb((Surface::FRenderOffscreen)onSurfaceRenderOffscreen, this);
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
			pAnimation->setDrawBackground(false);
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
	{
		pSurface->setRenderOffscreenCb((Surface::FRenderOffscreen)onSurfaceRenderOffscreen, this);
		ofAddListener(pSurface->m_eventOffscreenChanged, this, &AnimationComposition::onSurfaceOffscreenChanged);

	}
	
	m_shader.load("Shaders/animCompo.vert", "Shaders/animCompoMultiply.frag");
	allocateRenderBuffers();
	setRenderNormal(true);
}

//--------------------------------------------------------------
void AnimationComposition::VM_update(float dt)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
	 	(*it)->VM_update(dt);

	if (m_bAllocateRenderBuffers)
	{
		m_bAllocateRenderBuffers=false;
		Surface* pSurface = GLOBALS->getSurfaceMain();
		if (pSurface)
		{
			float w = pSurface->getOffscreen().getWidth();
			float h = pSurface->getOffscreen().getHeight();
			int nbSamples = pSurface->m_fboNbSamples;
		
			m_fboAnimation1.allocate(w,h, GL_RGBA,nbSamples);
			m_fboAnimation2.allocate(w,h, GL_RGBA,nbSamples);
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::VM_draw(float w, float h)
{
	if (m_bRenderNormal)
	{
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
	}
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
	if (pThis->m_bRenderNormal == false)
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

			fboFinal.begin();

			pThis->m_shader.begin();
			pThis->m_shader.setUniformTexture("tex1", pThis->m_fboAnimation1.getTextureReference(), 1);
			pThis->m_shader.setUniformTexture("tex2", pThis->m_fboAnimation2.getTextureReference(), 2);
			pThis->m_fboAnimation1.draw(0,0);
			pThis->m_shader.end();

			fboFinal.end();

		}
		else
		{
			fboFinal.begin();
			ofBackground(0);
			fboFinal.end();
		}
	}
}

//--------------------------------------------------------------
void AnimationComposition::onSurfaceOffscreenChanged(ofVec2f& dimensions)
{
	allocateRenderBuffers();
}

//--------------------------------------------------------------
void AnimationComposition::allocateRenderBuffers()
{
	m_bAllocateRenderBuffers = true;
}

//--------------------------------------------------------------
void AnimationComposition::handleMidiMessages()
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it){
	 	(*it)->handleMidiMessages();
	}
}

//--------------------------------------------------------------
void AnimationComposition::newMidiMessage(ofxMidiMessage& eventArgs)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it){
	 	(*it)->newMidiMessage(eventArgs);
	}
}

//--------------------------------------------------------------
void AnimationComposition::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);
	
	string name = e.getName();
	if (name == "renderNormal")
	{
		//setRenderNormal(m_bRenderNormal);
	}
}






