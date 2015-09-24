//
//  RadarFFElement_filled.h
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#pragma once
#include "RadarFFElement.h"

class RadarFFElement_filled : public RadarFFElement
{
	public:
	   	RadarFFElement_filled	(AnimationRadarFF* pParent, float x, float y);
	   	~RadarFFElement_filled	();

		void			updateCustom		(float dt);
		void 			draw				();

		ofVboMesh		m_mesh;
};
