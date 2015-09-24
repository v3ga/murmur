//
//  RadarFFElement_radial.cpp
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#include "RadarFFElement_radial.h"
#include "AnimationRadarFF.h"

//--------------------------------------------------------------
RadarFFElement_radial::RadarFFElement_radial(AnimationRadarFF* pParent, float x, float y) : RadarFFElement(pParent,x,y)
{
	m_nbSubdiv			= 60;
	m_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	m_mesh.setUsage(GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
RadarFFElement_radial::~RadarFFElement_radial()
{
}

//--------------------------------------------------------------
void RadarFFElement_radial::updateCustom(float dt)
{
	m_radiusWidth = 10*3;

	
	if (m_mesh.getVertices().size() == 0)
	{
		for (int i=0; i<m_nbSubdiv; i++)
		{
			for (int j=0;j<4;j++){
				m_mesh.addVertex(ofVec3f());
			}

			int offset = 4*i;
			m_mesh.addIndex(offset);
			m_mesh.addIndex(offset+1);
			m_mesh.addIndex(offset+2);
			
			m_mesh.addIndex(offset+2);
			m_mesh.addIndex(offset+3);
			m_mesh.addIndex(offset);
		}
	}
	
	float angleStep = (m_angleEnd-m_angleBegin)/(float)(m_nbSubdiv-1);
	float angle = 0.0f;
	ofVec2f A,B,C,D;
	for (int i=0; i<m_nbSubdiv; i++)
	{
		float angle1 = ofDegToRad(angle);
		float angle2 = ofDegToRad(angle+0.5*angleStep);

		A.set( m_radius*cos(angle1), m_radius*sin(angle1));
		B.set( (m_radius+m_radiusWidth)*cos(angle1), (m_radius+m_radiusWidth)*sin(angle1));
		C.set( (m_radius+m_radiusWidth)*cos(angle2), (m_radius+m_radiusWidth)*sin(angle2));
		D.set( m_radius*cos(angle2), m_radius*sin(angle2));


		m_mesh.setVertex(4*i, A);
		m_mesh.setVertex(4*i+1, B);
		m_mesh.setVertex(4*i+2, C);
		m_mesh.setVertex(4*i+3, D);


		angle+=angleStep;
	}
}

//--------------------------------------------------------------
void RadarFFElement_radial::draw()
{
	ofSetColor(m_color);
	ofPushMatrix();
	ofTranslate(m_pos);
	ofRotate(m_rot);
	m_mesh.draw();
	ofPopMatrix();
}