//
//  animationComposition.cpp
//  murmur
//
//  Created by Julien on 25/06/2015.
//
//

#include "animationComposition.h"
#include "animationsFactory.h"

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
	Animation* pAnimation = AnimationsFactory::create(name);
	if (pAnimation){
		pAnimation->setDrawBackground(false);
		m_animations.push_back(pAnimation);
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

//--------------------------------------------------------------
void AnimationComposition::onNewPacket(DevicePacket* pDevice, string deviceId, float x, float y)
{
	vector<Animation*>::iterator it = m_animations.begin();
	for ( ; it != m_animations.end(); ++it)
		(*it)->onNewPacket(pDevice,deviceId,x,y);
}


