//
//  animationRadarFF.cpp
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#include "animationRadarFF.h"
#include "RadarFFElement_filled.h"


//--------------------------------------------------------------
AnimationRadarFF::AnimationRadarFF(string name) : Animation(name)
{
}

//--------------------------------------------------------------
AnimationRadarFF::~AnimationRadarFF()
{
}

//--------------------------------------------------------------
void AnimationRadarFF::createUICustom()
{
    if (mp_UIcanvas)
    {
	}
}

//--------------------------------------------------------------
void AnimationRadarFF::VM_update(float dt)
{
	updateUIVolume();

	vector<RadarFFElement*>::iterator it = m_elements.begin();
	for ( ; it!=m_elements.end();++it)
	{
		RadarFFElement* p = *it;
		p->update(dt);
/*
		if (p->m_pos.z>=1000)
		{
			delete p;
			m_elements.erase(it);
		}
		else ++it;
*/
	}
}


//--------------------------------------------------------------
void AnimationRadarFF::VM_draw(float w, float h)
{
	m_w = w;
	m_h = h;
	
	drawBackground(0);

	ofPushStyle();
	ofNoFill();
	int nbElements = m_elements.size();
	for (int i=0;i<nbElements;i++)
	{
		m_elements[i]->draw();
	}
	ofPopStyle();
}


//--------------------------------------------------------------
void AnimationRadarFF::onVolumAccumEvent(string deviceId)
{
	Device* pDevice = getDevice(deviceId);
	if (pDevice)
	{
		RadarFFElement* pRadar = new RadarFFElement_filled(pDevice->m_pointSurface.x*m_w,pDevice->m_pointSurface.y*m_h);
		
		
		pRadar->m_angleBegin = 0;
		pRadar->m_angleEnd = 90;

		m_elements.push_back(pRadar);
	}
}

//--------------------------------------------------------------
void AnimationRadarFF::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	accumulateVolume(pDevicePacket->m_volume, deviceId);
}

