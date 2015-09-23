//
//  RadarFFElement_filled.cpp
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#include "RadarFFElement_filled.h"

RadarFFElement_filled::RadarFFElement_filled(float x, float y) : RadarFFElement(x,y)
{
	m_mesh.enableIndices();
	m_mesh.disableNormals();
	m_mesh.disableTextures();
	m_mesh.disableColors();
}

void RadarFFElement_filled::updateCustom(float dt)
{
	m_arc.clear();
	m_arc.arc(ofVec2f(), m_radius,m_radius, m_angleBegin, m_angleEnd, true,60);


	m_mesh.clear();
	m_mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	m_radiusWidth = 5;

	
	vector<ofPoint>& points = m_arc.getVertices();
	int nbPoints = points.size();
	
	ofPoint p,r;
	for (int i=0 ; i<nbPoints ; i++)
	{
		p = points[i];
		r = p.normalized();
		m_mesh.addVertex( m_pos+r*(m_radius) );
		m_mesh.addVertex( m_pos+r*(m_radius+m_radiusWidth) );
		m_mesh.addIndex(2*i);
		m_mesh.addIndex(2*i+1);
	}
}

void RadarFFElement_filled::draw()
{
	ofSetColor(m_color);
	m_mesh.draw();
}