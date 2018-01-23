//
//  animationDebug.cpp
//  murmur
//
//  Created by Julien on 18/01/2018.
//
//

#include "animationDebug.h"

//--------------------------------------------------------------
AnimationDebug::AnimationDebug(string name) : Animation(name)
{
	m_bHandlePitch = true;
}


//--------------------------------------------------------------
void AnimationDebug::createUICustom()
{
}

//--------------------------------------------------------------
void AnimationDebug::VM_update(float dt)
{
}

//--------------------------------------------------------------
void AnimationDebug::VM_draw(float w, float h)
{
	ofBackground(100);
	ofSetColor(255);
	ofDrawBitmapString("volume = "+ofToString(m_volume), 	5,20);
	ofDrawBitmapString("pitch = "+ofToString(m_pitch), 		5,35);
}

//--------------------------------------------------------------
void AnimationDebug::guiEvent(ofxUIEventArgs &e)
{
}

//--------------------------------------------------------------
void AnimationDebug::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float xNorm, float yNorm)
{
	if (pDevicePacket==0) return;

	m_volume = pDevicePacket->m_volume;
	m_pitch = pDevicePacket->m_pitch;
}

