//
//  RadarFFElement.cpp
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#include "RadarFFElement.h"
#include "animationRadarFF.h"

//--------------------------------------------------------------
RadarFFElement::RadarFFElement(AnimationRadarFF* pParent, float x, float y)
{
	mp_parent = pParent;

	m_age = 0.0f;

	m_color.set(255);
	
	m_pos.set(x,y);

	m_radius 		= 0.0f;
	m_radiusWidth 	= 0.0f;
	m_radiusSpeed	= 100.0f;
	
	m_rot = 0.0f;
	m_angleBegin = 0.0f;
	m_angleEnd = 360.0f;
	
	m_rotSpeed = ofRandom(-20,20);
	
	
}

//--------------------------------------------------------------
RadarFFElement::~RadarFFElement()
{
}

//--------------------------------------------------------------
void RadarFFElement::update(float dt)
{
	m_age += dt;
	m_radius += m_radiusSpeed*dt;
	m_rot += m_rotSpeed*dt;
	if (m_age >= 0.9f*mp_parent->m_ageMax){
		m_color.a = (unsigned char)(255.0f*(m_age-0.9f*mp_parent->m_ageMax)/(0.1f*mp_parent->m_ageMax));
	}

	updateCustom(dt);
}

//--------------------------------------------------------------
void RadarFFElement::updateCustom(float dt)
{
	m_arc.clear();
	m_arc.arc(m_pos, m_radius,m_radius, m_angleBegin, m_angleEnd, true);
}


//--------------------------------------------------------------
void RadarFFElement::draw()
{
   ofSetColor(m_color); // TEMP , adjust to device color
   //ofEllipse(m_pos.x, m_pos.y, 2.0*m_radius, 2.0*m_radius);
   m_arc.draw();
}


