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
	   	RadarFFElement_filled	(float x, float y);

		void			updateCustom		(float dt);
		void 			draw				();

		ofMesh			m_mesh;
};
