//
//  RadarFFElement_radial.h
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#pragma once
#include "RadarFFElement.h"

class RadarFFElement_radial : public RadarFFElement
{
	public:
	   	RadarFFElement_radial	(AnimationRadarFF* pParent, float x, float y);
	   	~RadarFFElement_radial	();

		void			updateCustom		(float dt);
		void 			draw				();

		ofVboMesh		m_mesh;
		int				m_nbSubdiv;
};
