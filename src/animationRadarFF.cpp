//
//  animationRadarFF.cpp
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#include "animationRadarFF.h"
#include "RadarFFElement_filled.h"
#include "RadarFFElement_dotted.h"
#include "RadarFFElement_radial.h"


//--------------------------------------------------------------
AnimationRadarFF::AnimationRadarFF(string name) : Animation(name)
{
	m_ageMax			 = 5.0f;
	m_speed				 = 100.0f;

	m_filled_radiusWidth = 5;
	m_dotted_radiusWidth = 5;
	m_radial_radiusWidth = 5;
	
	
	m_properties.add( new classProperty_float("age max", 1.0f, 10.0f, &m_ageMax) );
	m_properties.add( new classProperty_float("speed", 50.0f, 200.0f, &m_speed) );

	// Filled
	m_properties.add( new classProperty_float("radius width filled", 1.0f, 20.0f, &m_filled_radiusWidth) );

	// Dotted
	m_properties.add( new classProperty_float("radius width dotted", 1.0f, 20.0f, &m_dotted_radiusWidth) );

	// Radial
	m_properties.add( new classProperty_float("radius width radial", 1.0f, 20.0f, &m_radial_radiusWidth) );
}

//--------------------------------------------------------------
AnimationRadarFF::~AnimationRadarFF()
{
	deleteElements();
}

//--------------------------------------------------------------
void AnimationRadarFF::createUICustom()
{
    if (mp_UIcanvas)
    {
		float dim = 16;
		int widthDefault = 320;

       //mp_UIcanvas->addToggle("colorFromDevice", &m_isColorFromDevice);

		addUISlider( m_properties.getFloat("age max") );
		addUISlider( m_properties.getFloat("speed") );

	    mp_UIcanvas->addWidgetDown( new ofxUILabel("radar filled",OFX_UI_FONT_MEDIUM) );
    	mp_UIcanvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );
		addUISlider( m_properties.getFloat("radius width filled") );

	    mp_UIcanvas->addWidgetDown( new ofxUILabel("radar dotted",OFX_UI_FONT_MEDIUM) );
    	mp_UIcanvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );
		addUISlider( m_properties.getFloat("radius width dotted") );

	    mp_UIcanvas->addWidgetDown( new ofxUILabel("radar radial",OFX_UI_FONT_MEDIUM) );
    	mp_UIcanvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );
		addUISlider( m_properties.getFloat("radius width radial") );
		

		// Filled
		// addUISlider( m_properties.getFloat("intensity") );
		
		
	
	}
}

//--------------------------------------------------------------
void AnimationRadarFF::VM_enter()
{
	setDrawBackground();
	if (m_bResetOnEnter)
	{
		deleteElements();
	}
}


//--------------------------------------------------------------
void AnimationRadarFF::VM_update(float dt)
{
	updateUIVolume();

	vector<RadarFFElement*>::iterator it = m_elements.begin();
	for ( ; it!=m_elements.end();)
	{
		RadarFFElement* p = *it;
		p->update(dt);

		if (p->m_age>=m_ageMax)
		{
			delete p;
			m_elements.erase(it);
		}
		else ++it;

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
	playSound(deviceId);

	Device* pDevice = getDevice(deviceId);
	if (pDevice)
	{
		RadarFFElement* pRadar = 0;
	
		int rnd = (int) ofRandom(3);
		if (rnd == 0)
		{
			pRadar = new RadarFFElement_filled(this,pDevice->m_pointSurface.x*m_w,pDevice->m_pointSurface.y*m_h);
			pRadar->m_radiusWidth = m_filled_radiusWidth;
			pRadar->m_rot = 90 * (int)(ofRandom(3))*90;
			pRadar->m_angleBegin = 90;
			pRadar->m_angleEnd = 360;

		}
		else if (rnd == 1)
		{
			RadarFFElement_dotted* pRadar_dotted = new RadarFFElement_dotted(this,pDevice->m_pointSurface.x*m_w,pDevice->m_pointSurface.y*m_h);
			pRadar_dotted->m_radiusWidth = m_dotted_radiusWidth;
			pRadar_dotted->m_angleBegin = 0;
			pRadar_dotted->m_angleEnd = 360;
			pRadar_dotted->m_ratioFillEmpty = ofRandom(0.5f,0.95f);
	
			pRadar = (RadarFFElement*) pRadar_dotted;
	
		}
		else if (rnd == 2)
		{
		
			pRadar = new RadarFFElement_radial(this,pDevice->m_pointSurface.x*m_w,pDevice->m_pointSurface.y*m_h);
			pRadar->m_radiusWidth = m_radial_radiusWidth;
			pRadar->m_angleBegin = 0;
			pRadar->m_angleEnd = 360;

		}

		if (pRadar)
		{
			pRadar->m_color =  m_isColorFromDevice ? m_lastPackeColor[deviceId] : ofColor(255) ;

			m_elements.push_back(pRadar);
		}
	
		

	}
}

//--------------------------------------------------------------
void AnimationRadarFF::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	//m_lastPackeColor[deviceId] = pDevicePacket->m_color;
	accumulateVolume(pDevicePacket->m_volume, deviceId);
	if (pDevicePacket->m_volume >= m_volValuesMeanTh)
	{
	//	onVolumAccumEvent(deviceId);
	}

}

//--------------------------------------------------------------
void AnimationRadarFF::deleteElements()
{
	vector<RadarFFElement*>::iterator it = m_elements.begin();
	for ( ; it != m_elements.end() ; ++it){
		delete *it;
	}
	m_elements.clear();
}


