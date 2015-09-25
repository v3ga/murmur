//
//  LineFFElement.cpp
//  murmur
//
//  Created by Julien on 25/09/2015.
//
//

#include "LineFFElement.h"
#include "animationLinesFF.h"

LineFFElement::LineFFElement(AnimationLinesFF* pParent, ofVec3f pos, ofVec3f dir)
{
	mp_parent = pParent;

	m_pos = pos;
	m_dir = dir;
	m_rot = ofRandom(360.0f);
	
	mp_meshPlane = &pParent->m_meshPlane;
	

}

void LineFFElement::update(float dt)
{
	m_pos += m_dir*mp_parent->m_dirSpeed*dt;
	m_rot += mp_parent->m_rotSpeed*dt;

	if (mp_parent->m_bRadiusDirect)
		m_radius += (mp_parent->m_radius-m_radius)*0.8*dt;
}

void LineFFElement::draw()
{
	ofPushMatrix();
	ofTranslate(m_pos);
 
	ofTranslate(m_radius*cos(ofDegToRad(m_rot)), m_radius*sin(ofDegToRad(m_rot)));
	ofRotateZ(m_rot);
	mp_meshPlane->draw();
	ofPopMatrix();
}
