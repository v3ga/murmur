//
//  RadarFFElement_dotted.h
//  murmur
//
//  Created by Julien on 23/09/2015.
//
//

#pragma once
#include "RadarFFElement.h"

class RadarFFElement_dotted : public RadarFFElement
{
	public:
	   	RadarFFElement_dotted	(AnimationRadarFF* pParent, float x, float y);
	   	~RadarFFElement_dotted	();

		void			updateCustom		(float dt);
		void 			draw				();

		float			m_ratioFillEmpty;
		int				m_nbSubdiv;
		ofVboMesh*		mp_meshes;
};
