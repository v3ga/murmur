//
//  RadarFFElement_filled.cpp
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#include "RadarFFElement_filled.h"
#include "AnimationRadarFF.h"

//--------------------------------------------------------------
RadarFFElement_filled::RadarFFElement_filled(AnimationRadarFF* pParent, float x, float y) : RadarFFElement(pParent,x,y)
{
	m_mesh.enableIndices();
	m_mesh.disableNormals();
	m_mesh.disableTextures();
	m_mesh.disableColors();
	m_mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
}

//--------------------------------------------------------------
RadarFFElement_filled::~RadarFFElement_filled()
{
}

//--------------------------------------------------------------
void RadarFFElement_filled::updateCustom(float dt)
{
	m_radiusWidth = 5;

	// Update arc
	m_arc.clear();
	m_arc.arc(ofVec2f(), m_radius,m_radius, m_angleBegin, m_angleEnd, true, 60);

	// Create points if necessary
	vector<ofPoint>& points = m_arc.getVertices();
	int nbPoints = points.size();
	
	// Create mesh
	if (m_mesh.getVertices().size()==0)
	{
		for (int i=0 ; i<2*nbPoints ; i++)
		{
			m_mesh.addVertex( ofVec3f() );
			m_mesh.addIndex(i);
		}

	}

	// Update
	ofPoint p,r;
	for (int i=0 ; i<nbPoints ; i++)
	{
		p = points[i];
		r = p.normalized();
		m_mesh.setVertex( 2*i, r*(m_radius) );
		m_mesh.setVertex( 2*i+1, r*(m_radius+m_radiusWidth) );
	}

}

//--------------------------------------------------------------
void RadarFFElement_filled::draw()
{
	ofSetColor(m_color);
	ofPushMatrix();
	ofTranslate(m_pos);
	ofRotate(m_rot);
	m_mesh.draw();
	ofPopMatrix();
}