//
//  RadarFFElement.cpp
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#include "RadarFFElement.h"

//--------------------------------------------------------------
RadarFFElement::RadarFFElement(float x, float y)
{
	m_type = TYPE_SIMPLE;

	m_color.set(255);
	
	m_pos.set(x,y);

	m_radius 		= 0.0f;
	m_radiusWidth 	= 0.0f;
	m_radiusSpeed	= 100.0f;
	
	m_rot = 0.0f;
	m_angleBegin = 0.0f;
	m_angleEnd = 360.0f;
	
	
}

//--------------------------------------------------------------
RadarFFElement::~RadarFFElement()
{
}

//--------------------------------------------------------------
void RadarFFElement::update(float dt)
{
	m_radius += m_radiusSpeed*dt;
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
	if (m_type == TYPE_SIMPLE)
	{
		ofSetColor(m_color); // TEMP , adjust to device color
		//ofEllipse(m_pos.x, m_pos.y, 2.0*m_radius, 2.0*m_radius);
		m_arc.draw();
	}
}


