//
//  RadarFFElement_dotted.cpp
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#include "RadarFFElement_dotted.h"
#include "AnimationRadarFF.h"

//--------------------------------------------------------------
RadarFFElement_dotted::RadarFFElement_dotted(AnimationRadarFF* pParent, float x, float y) : RadarFFElement(pParent,x,y)
{
	m_ratioFillEmpty	= 0.5f;
	m_nbSubdiv			= 20;
	mp_meshes 			= 0;
	
}

//--------------------------------------------------------------
RadarFFElement_dotted::~RadarFFElement_dotted()
{
	delete[] mp_meshes;
	mp_meshes = 0;
}

//--------------------------------------------------------------
void RadarFFElement_dotted::updateCustom(float dt)
{
	m_radiusWidth = 5;

	// Create the list of meshes
	int nbPointsPerArc = 10;
	
	if (mp_meshes==0)
	{
		mp_meshes = new ofVboMesh[m_nbSubdiv];

		for (int i=0; i<m_nbSubdiv; i++)
		{
			mp_meshes[i].setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
			mp_meshes[i].setUsage(GL_DYNAMIC_DRAW);


			for (int j=0; j<2*nbPointsPerArc; j++)
			{
			 	mp_meshes[i].addVertex(ofVec3f());
			 	mp_meshes[i].addIndex(j);
			}
		}
	}
	
	if (mp_meshes)
	{
		float angleStep = (m_angleEnd-m_angleBegin) / (float)m_nbSubdiv;
		float angle = 0.0f;
		float angleBegin = 0.0f;
		float angleEnd = 0.0f;
		ofVec2f pArc0,pArc1,pArc;

		for (int i=0; i<m_nbSubdiv; i++)
		{
		 	angleBegin = angle;
			angleEnd = angle + m_ratioFillEmpty*angleStep;

			float angleStep2 	= (angleEnd-angleBegin)/float(nbPointsPerArc-1);
			float angle2 		= angleBegin;
			for (int j=0; j<nbPointsPerArc; j++)
			{
				pArc.set(cos(ofDegToRad(angle2)), sin(ofDegToRad(angle2)));
				pArc0 = m_radius * pArc;
				pArc1 = (m_radius+m_radiusWidth) * pArc;

			 	mp_meshes[i].setVertex(2*j, 	pArc0);
			 	mp_meshes[i].setVertex(2*j+1, 	pArc1);
				
				angle2+=angleStep2;
			}
		 
		 	angle+=angleStep;
		}
	
	}
}

//--------------------------------------------------------------
void RadarFFElement_dotted::draw()
{
	ofPushMatrix();
	ofTranslate(m_pos);
	ofRotate(m_rot);
	for (int i=0; i<m_nbSubdiv; i++)
	{
		mp_meshes[i].draw();
	}
	ofPopMatrix();
}

